#include "AudioStream.hpp"
#include <queue>
#include <chrono>
#include <thread>



AudioStreamer::AudioStreamer(vector<int> audio_index){
    audio_index_inner = audio_index;
    audio_count = audio_index_inner.size();
    startMultiCapture();

}

AudioStreamer::AudioStreamer( vector<int> audio_index, int sample_rate, int frame){
    SAMPLE_RATE = sample_rate;
    FRAMES = frame;
    audio_index_inner = audio_index;
    audio_count = audio_index.size();
    startMultiCapture();

}


AudioStreamer:: ~AudioStreamer(){

    stopMultiCapture();

}


void AudioStreamer::captureFrame(int index)
{
      PaStream* Inputstream = vector_stream[index];
      Pa_StartStream(Inputstream);
      PaError err; 


      while(true){

        long toRead = Pa_GetStreamReadAvailable(Inputstream);
        
        if (toRead > FRAMES) {
    
            printf("available: %ld frames_per_buffer: %d index %d \n", toRead, FRAMES, index);
        
            // You may get underruns or overruns if the output is not primed by PortAudio.
            err = Pa_ReadStream( Inputstream, vector_buffer[index], FRAMES);
            if (err == paNoError){
                std::cout<<"read successfully"<<std::endl;
            }
        }

        zmq::message_t request(numbytes);
		memcpy(request.data(), vector_buffer[index], numbytes);
        socket_vector[index]->send(request);
      }

      Pa_CloseStream(Inputstream);
}



bool AudioStreamer::startMultiCapture()
{


    PaError err; 
    err = Pa_Initialize();
    if (err != paNoError) {
            std::cerr << "Failed to initialize PortAudio: " << Pa_GetErrorText(err) << std::endl;
            return false;
        }


    PaStream*  input_stream;
    thread* t;
    PaStreamParameters inputParameters;
    std::mutex *m_p;
    float* buffer;
    
    zmq::context_t *c;
    zmq::socket_t *s;


    numsamples = FRAMES;
    numbytes = numsamples * sizeof(float);


    // initilize the stream
    for(int i = 0; i < audio_count; i++)
    {
 
        inputParameters.device = audio_index_inner[i];
        inputParameters.channelCount = 1;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
        inputParameters.hostApiSpecificStreamInfo = nullptr;

      
        err = Pa_OpenStream(&input_stream, &inputParameters, nullptr, SAMPLE_RATE, FRAMES, paNoFlag, nullptr, nullptr);

        if (err != paNoError) {
            std::cout << "Failed to initialize PortAudio library: "
                  << Pa_GetErrorText(err) << std::endl;
            return false;
        }   




        buffer = (float*) malloc(numbytes);
        vector_buffer.push_back(buffer);
        vector_stream.push_back(input_stream);

        vector_inparameters.push_back(inputParameters);

        // create new context

        c = new zmq::context_t(1);
        context_vector.push_back(c);

        s = new zmq::socket_t(*c, ZMQ_PUSH);

        int immediate = 1;
        s->setsockopt(ZMQ_IMMEDIATE, &immediate, sizeof(immediate));


        string IPinfo = "tcp://" + IP + ":" + std::to_string(4444+i);
        s->connect(IPinfo);
        socket_vector.push_back(s);

        m_p = new std::mutex;
        mutex_vector.push_back(m_p);

        //Make thread instance
        t = new thread(&AudioStreamer::captureFrame, this, i);
        // Put thread to the vector
        vector_thread.push_back(t);
   
    }

    return true;

}


bool  AudioStreamer::stopMultiCapture(){

    // for(int i=0; i<audio_count; i++){
    //     Pa_CloseStream(vector_stream[i]);
    // }

    for(int i=0; i<audio_count;i++){
        free(vector_buffer[i]);
    }
    Pa_Terminate();
    return true;
}
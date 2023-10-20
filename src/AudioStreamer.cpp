#include "AudioStreamer.hpp"
#include <queue>
#include <chrono>
#include <thread>


int AudioStreamer::captureFrame(const void *inputBuffer, void *outputBuffer, unsigned long frameCount,
const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{

    CallbackPara* params = static_cast<CallbackPara*>(userData);   
    

    int index = (*params).index;
    AudioStreamer* asp =(*params).asp;
    int numbytes = asp->numbytes;

    std::cout<<"send successfully"<<std::endl;

    zmq::message_t request(numbytes);
    memcpy(request.data(), inputBuffer, numbytes);
    asp->socket_vector[index]->send(request, ZMQ_NOBLOCK);


    return 0;    
}



AudioStreamer::AudioStreamer(){
    audio_count = 1;
    USE_DEFAULT = true;
    IP="192.168.137.1";


}


AudioStreamer::AudioStreamer(int sample_rate, int frame){
    SAMPLE_RATE = sample_rate;
    SAMPLE_FRAME  = frame;
    audio_count = 1;
    USE_DEFAULT = true;
    IP="192.168.137.1";


}

AudioStreamer::AudioStreamer(vector<int> audio_index){
    audio_index_inner = audio_index;
    audio_count = audio_index_inner.size();
    USE_DEFAULT = false;
    IP="192.168.137.1";


}

AudioStreamer::AudioStreamer( vector<int> audio_index, int sample_rate, int frame){
    audio_index_inner = audio_index;
    audio_count = audio_index.size();
    SAMPLE_RATE = sample_rate;
    SAMPLE_FRAME = frame;
    USE_DEFAULT = false;
    IP="192.168.137.1";


}


AudioStreamer:: ~AudioStreamer(){


    for(int i=0; i<audio_count;i++){
        free(vector_buffer[i]);
        // delete context_vector[i];
        // delete socket_vector[i];
        // 智能指针无法delete
        
        delete vector_callback[i];
    }

}



bool AudioStreamer::startMultiCapture()
{


  
    zmq::context_t *c;
    zmq::socket_t *s;
    CallbackPara *cp;

    float* buffer;
    numsamples = SAMPLE_FRAME;
    numbytes = numsamples * sizeof(float);

    PaError err = Pa_Initialize();

    if (err != paNoError) {
    std::cout << "PortAudio 初始化失败: " << Pa_GetErrorText(err) << std::endl;
    }
 

    // initilize the stream
    for(int i = 0; i < audio_count; i++)
    {
 
        //inputParameters.device = audio_index_inner[i];
        buffer = (float*)malloc(numbytes); 
        vector_buffer.push_back(buffer);


        PaStreamParameters inputParameters;

        if (USE_DEFAULT){
            inputParameters.device = Pa_GetDefaultInputDevice();
        }
        else{
            inputParameters.device = audio_index_inner[i];
        }
    
        inputParameters.channelCount = CHANNEL_COUNT;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighOutputLatency;
        inputParameters.hostApiSpecificStreamInfo = nullptr;
        vector_pa.push_back(inputParameters);
            

     


        // create new context
        c = new zmq::context_t(1);
        context_vector.push_back(c);
        s = new zmq::socket_t(*c, ZMQ_PUSH);
        int immediate = 1;
        s->setsockopt(ZMQ_IMMEDIATE, &immediate, sizeof(immediate));
        string IPinfo = "tcp://" + IP + ":" + std::to_string(4444+i);
        s->connect(IPinfo);
        socket_vector.push_back(s);


        cp = new CallbackPara(i, this);
        vector_callback.push_back(cp);


        PaStream* stream = nullptr;
        PaError err;
        err = Pa_OpenStream(&stream, &inputParameters, nullptr, SAMPLE_RATE, SAMPLE_FRAME, paClipOff, captureFrame, cp); 
        if (err != paNoError) {
            std::cout << "open stream fail: " << Pa_GetErrorText(err) << std::endl;
            Pa_Terminate();
        }

        err = Pa_StartStream(stream); 
        vector_stream.push_back(stream);  

    }


    return true;

}


bool  AudioStreamer::stopMultiCapture(){

    for(int i=0; i<audio_count; i++){
        Pa_StopStream(vector_stream[i]); 
        Pa_CloseStream(vector_stream[i]);
    }


    Pa_Terminate();
    return true;
}
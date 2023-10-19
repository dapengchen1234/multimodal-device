#include "AudioStreamer.hpp"
#include <queue>
#include <chrono>
#include <thread>



AudioStreamer::AudioStreamer(){
    audio_count = 1;
    USE_DEFAULT = true;

    startMultiCapture();

}


AudioStreamer::AudioStreamer(int sample_rate, int frame){
    SAMPLE_RATE = sample_rate;
    SAMPLE_FRAME  = frame;
    audio_count = 1;
    USE_DEFAULT = true;

    startMultiCapture();

}

AudioStreamer::AudioStreamer(vector<int> audio_index){
    audio_index_inner = audio_index;
    audio_count = audio_index_inner.size();
    USE_DEFAULT = false;
    startMultiCapture();

}

AudioStreamer::AudioStreamer( vector<int> audio_index, int sample_rate, int frame){
    audio_index_inner = audio_index;
    audio_count = audio_index.size();
    SAMPLE_RATE = sample_rate;
    SAMPLE_FRAME = frame;
    USE_DEFAULT = false;
    startMultiCapture();

}


AudioStreamer:: ~AudioStreamer(){

    stopMultiCapture();

}


void AudioStreamer::captureFrame(int index)
{

    PaStream* stream = nullptr;
    PaError err;

    // 初始化PortAudio
     std::cout<<"here1"<<std::endl;


    // 打开流
    err = Pa_OpenStream(&stream, &vector_pa[index], nullptr, SAMPLE_RATE, SAMPLE_FRAME, paClipOff, nullptr, nullptr);
    if (err != paNoError) {
        std::cout << "打开流失败: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
    }


    while(true){
        err = Pa_ReadStream(stream, vector_buffer[index], SAMPLE_FRAME);

        if (err != paNoError) {
            std::cout << "录音过程中发生错误: " << Pa_GetErrorText(err) << std::endl;
            break;
        }else{
            std::cout<< "read successfully" <<std::endl;
        }
    }


    // 停止录音并关闭流
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cout << "停止录音失败: " << Pa_GetErrorText(err) << std::endl;
    }
    Pa_CloseStream(stream);


}



bool AudioStreamer::startMultiCapture()
{


  
    zmq::context_t *c;
    zmq::socket_t *s;

    float* buffer;
    thread* t;
    numsamples = SAMPLE_FRAME;
    numbytes = numsamples * sizeof(float);

    PaError err = Pa_Initialize();

    std::cout<<"here0"<<std::endl;

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
            
            
        PaStream* stream = nullptr;
        PaError err;

        // err = Pa_OpenStream(&stream, &inputParameters, nullptr, SAMPLE_RATE, SAMPLE_FRAME, paClipOff, nullptr, nullptr);
   
        // create new context
        c = new zmq::context_t(1);
        context_vector.push_back(c);
        s = new zmq::socket_t(*c, ZMQ_PUSH);

        int immediate = 1;
        s->setsockopt(ZMQ_IMMEDIATE, &immediate, sizeof(immediate));


        string IPinfo = "tcp://" + IP + ":" + std::to_string(4444+i);
        s->connect(IPinfo);
        socket_vector.push_back(s);
      
        t = new thread(&AudioStreamer::captureFrame, this, i);
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
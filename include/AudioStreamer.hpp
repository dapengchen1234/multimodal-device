#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include "portaudio.h"
#include <zmq.hpp>
#include <mutex>


using namespace std;


class AudioStreamer{
   
    public:
        
        //
        AudioStreamer();

        AudioStreamer(int sample_rate, int sample_frame);


        //Constructor for default audio devices
        AudioStreamer(vector<int> audio_index);
        //Constructor with specific parameters
        AudioStreamer(vector<int> audio_index, int sample_rate, int sample_frame);
        //Destructor for releasing resouce(s)
        ~AudioStreamer();
        //
        vector<int>  audio_index_inner; 
        vector<thread*> vector_thread; 
        vector<float*> vector_buffer;
        vector<PaStreamParameters> vector_pa;

        vector<zmq::context_t*> context_vector;
        vector<zmq::socket_t*> socket_vector;
        vector<std::mutex*> mutex_vector; 


        int audio_count;
        int numbytes, numsamples;
        

    private:
        // private parameters
        int SAMPLE_RATE = 16000;
        int SAMPLE_FRAME = 1280;
        int CHANNEL_COUNT = 1;
        bool USE_DEFAULT;

        std::string IP;
    

        // initialize and start the camera capturing process 
        bool startMultiCapture();
        // release all camera capture resources
        bool stopMultiCapture();
        // main camera capturing process which will be done by the thread(s)
        void captureFrame(int index);


};

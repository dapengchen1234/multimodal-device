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
        //Constructor for default audio devices
        AudioStreamer(vector<int> audio_index);
        //Constructor with specific parameters
        AudioStreamer(vector<int> audio_index, int sample_rate, int frame);
        //Destructor for releasing resouce(s)
        ~AudioStreamer();
        //
        vector<int>  audio_index_inner; 
        vector<PaStream*> vector_stream;
        vector<PaStreamParameters> vector_inparameters; 
        vector<thread*> vector_thread; 
        vector<float*> vector_buffer;

        vector<zmq::context_t*> context_vector;
        vector<zmq::socket_t*> socket_vector;
        vector<std::mutex*> mutex_vector; 


        int audio_count;
        int numbytes, numsamples;
        

    private:
        // private parameters
        int SAMPLE_RATE = 16000;
        int FRAMES = 1280;
        std::string IP;
    

        // initialize and start the camera capturing process 
        bool startMultiCapture();
        // release all camera capture resources
        bool stopMultiCapture();
        // main camera capturing process which will be done by the thread(s)
        void captureFrame(int index);


};

#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <opencv2/opencv.hpp>
#include<zmq.hpp>
#include <mutex>


using namespace std;

class CameraStreamer{
    public:
        //this holds camera stream urls
        vector<string> camera_source;
        //this holds usb camera indices
        vector<int> camera_index;
        //this holds OpenCV VideoCapture pointers
        vector<cv::VideoCapture*> camera_capture;
        //this holds queue(s) which hold images from each camera
        vector<std::vector<uchar>> frames;
        //this holds thread(s) which run the camera capture process
        vector<thread*> camera_thread;
        // 
        vector<zmq::context_t*> context_vector;

        vector<zmq::socket_t*> socket_vector;   

        vector<std::mutex*> mutex_vector; 
        
        //Constructor for IP Camera capture
        CameraStreamer(vector<string> source);
        //Constructor for USB Camera capture
        CameraStreamer(vector<int> index);
        //Destructor for releasing resource(s)
        ~CameraStreamer();
        void setbuffer(int index, std::vector<uchar>&);
        void getbuffer(int index, std::vector<uchar>&);

    private:

        bool isUSBCamera;
        int camera_count;
        std::string IP;
        // zmq::context_t context_t; 
        //initialize and start the camera capturing process(es)
        void startMultiCapture();
        //release all camera capture resource(s)
        void stopMultiCapture();
        //main camera capturing process which will be done by the thread(s)
        void captureFrame(int index);

      
};



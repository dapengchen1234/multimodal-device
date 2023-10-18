#include "CameraStreamer.hpp"
#include <queue>
#include <chrono>
#include <thread>
#include <atomic>





using namespace cv;


std::atomic<bool> shouldTerminate(false);


CameraStreamer::CameraStreamer(vector<string> stream_source)
{
    camera_source = stream_source;
    camera_count = camera_source.size();
    isUSBCamera = false;
    IP="192.168.137.1";

    startMultiCapture();
}


CameraStreamer::CameraStreamer(vector<int> capture_index)
{
    camera_index = capture_index;
    camera_count = capture_index.size();
    isUSBCamera = true;
    IP="192.168.137.1";

    startMultiCapture();
}

CameraStreamer::~CameraStreamer()
{

    shouldTerminate = true;
    for(int i=0; i<camera_thread.size(); i++)
    {
        camera_thread[i]->join();
    }
    
   
    stopMultiCapture();
}



void CameraStreamer::setbuffer(int index, std::vector<uchar>& buffer){
     std::lock_guard<std::mutex> lock(*(mutex_vector[index]));
     frames[index] = buffer; 
}
void CameraStreamer::getbuffer(int index, std::vector<uchar>& buffer){
    std::lock_guard<std::mutex> lock(*(mutex_vector[index]));
     buffer = frames[index];  
}



void CameraStreamer::captureFrame(int index)
{
    VideoCapture *capture = camera_capture[index];

    while (!shouldTerminate)
    {
        cv::Mat frame;       
        //Grab frame from camera capture
        (*capture) >> frame;
        string info = "extract the image"+ std::to_string(index);
        std::cout<<info<<std::endl;
        // sleep 

        //frame.release();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        std::vector<uchar> buffer; 
		std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 80};
		cv::imencode(".jpg", frame, buffer, params);

        // std::lock_guard<std::mutex> lock(mtx);
        // setbuffer(index, buffer);

        zmq::message_t request(buffer.size());
		memcpy(request.data(), buffer.data(),buffer.size());
        socket_vector[index]->send(request);

    }
}



void CameraStreamer::startMultiCapture()
{
    VideoCapture *capture;
    thread *t;
    zmq::context_t *c;
    zmq::socket_t  *s;
    std::mutex *m_p;


    for (int i = 0; i < camera_count; i++)
    {
    //Make VideoCapture instance
        if (!isUSBCamera){
        string url = camera_source[i];
        capture = new VideoCapture(url);
        cout << "Camera Setup: " << url << endl;
        }
        else{
            int idx = camera_index[i];
            capture = new VideoCapture(idx);
            capture->set(CAP_PROP_FRAME_WIDTH, 640.0);
            capture->set(CAP_PROP_FRAME_HEIGHT, 480.0);
            capture->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

            cout << "Camera Setup: " << to_string(idx) << endl;
        }
 
        //Put VideoCapture to the vector
        camera_capture.push_back(capture);
    

        //Make a mat install 
        cv::Mat m;
        
        //Put mat to the vector
        frames.push_back(m);

        //ii
        c = new zmq::context_t(1);
        context_vector.push_back(c);

        s = new zmq::socket_t(*c, ZMQ_PUSH);


        int immediate = 1;
        s->setsockopt(ZMQ_IMMEDIATE, &immediate, sizeof(immediate));

        string IPinfo = "tcp://" + IP + ":" + std::to_string(5555+i);
        s->connect(IPinfo);
        socket_vector.push_back(s);

        m_p = new std::mutex;
        mutex_vector.push_back(m_p);

        //Make thread instance
        t = new thread(&CameraStreamer::captureFrame, this, i);
        //Put thread to the vector
        camera_thread.push_back(t);
        


    }
}



void CameraStreamer::stopMultiCapture()
{
    VideoCapture *cap;
    for (int i = 0; i < camera_count; i++)
    {
        cap = camera_capture[i];
        if (cap->isOpened())
        {
         //Relase VideoCapture resource
         cap->release();
         cout << "Capture " << i << " released" << endl;
        }
    }
}
#include "CameraStreamer.hpp"
#include <opencv2/opencv.hpp>
#include<zmq.hpp>
#include <chrono>
#include <thread>


int main()
{
	cv::VideoCapture temp_camera;
	int maxTested = 10;
    int count=0;
	vector<int> capture_index;
	for (int i = 0; i < maxTested; i++){
		cv::VideoCapture temp_camera(i);
		bool res = (temp_camera.isOpened());
        if (res) 
		{
			capture_index.push_back(i);
			count++;
		}
	}


	// make an instance of cameraStreamer

	CameraStreamer cam(capture_index);

	// build a tunel for image transport

	std::vector<uchar> buffer; 
	while(true){

	
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		for(int i=0; i<capture_index.size();i++)
		{
			
			cam.getbuffer(i, buffer);

			zmq::message_t request(buffer.size());
			memcpy(request.data(), buffer.data(),buffer.size());
			cam.socket_vector[i]->send(request, ZMQ_NOBLOCK);

			// if (i==0) {
			// 	s1.send(request);
			// 	std::cout<<"send the frame 1"<<std::endl;
			// }

			// if(i==1){
			// 	s2.send(request);
			// 	std::cout<<"send the frame 2"<<std::endl;
			// }

		 }
	}

	return 0;

}

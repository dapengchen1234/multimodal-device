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


	while(true){

		std::cout<<"get the frame"<<std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	}

	return 0;

}

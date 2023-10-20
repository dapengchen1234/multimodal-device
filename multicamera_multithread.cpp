#include "CameraStreamer.hpp"
#include "AudioStreamer.hpp"
#include <opencv2/opencv.hpp>
#include<zmq.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <portaudio.h>
#include <conio.h> // 需要包含此头文件



int stop(){

  std::cout << "Press ESC to continue..." << std::endl;
  while (true) {
    if (_kbhit()) { // 检测是否有键盘输入
      char key = _getch(); // 获取键盘输入

      if (key == 27) { // 判断是否为ESC键的ASCII码值
        break; // 跳出循环
      }
    }
  }
  std::cout << "Continued!" << std::endl;

  return 0;
}

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

	

	// CameraStreamer cam(capture_index);
	// cam.startMultiCapture();
	// stop();
    AudioStreamer  as;
	// build a tunel for image transport
    as.startMultiCapture();
	stop();
	as.stopMultiCapture();
	stop();
}





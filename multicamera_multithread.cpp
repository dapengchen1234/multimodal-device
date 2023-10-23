#include "CameraStreamer.hpp"
#include "AudioStreamer.hpp"
#include <opencv2/opencv.hpp>
#include<zmq.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <portaudio.h>
#include <termios.h>

bool checkKeyboardInput() {
    struct termios oldSettings, newSettings;
    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int ready = select(1, &readfds, NULL, NULL, &timeout);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);

    if (ready == -1) {
        return false; // Error during select()
    }
    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        return true; // Keyboard input is available
    }
    return false; // No keyboard input
}


int stop() {

    while (true) {
        if (checkKeyboardInput()) { // Get keyboard input
            int key = getchar(); // Get ASCII value of the input

            if (key == 27) { // Check if it's the ESC key
                break; // Break out of the loop
            }
        }
    }

    printf("Continued!");

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

	

	CameraStreamer cam(capture_index);
	cam.startMultiCapture();	
    AudioStreamer  as;
    as.startMultiCapture();

    stop();
	as.stopMultiCapture();
    cam.stopMultiCapture();

    // build a tunel for image transport


}





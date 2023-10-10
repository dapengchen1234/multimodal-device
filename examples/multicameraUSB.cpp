#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;



int countCameras()
{
	cv::VideoCapture temp_camera;
	int maxTested = 10;
    int count=0;
	for (int i = 0; i < maxTested; i++){
		cv::VideoCapture temp_camera(i);
		bool res = (temp_camera.isOpened());
        if (res) count++;
	}

	return count;
}

int main(){


    VideoCapture cap0 = VideoCapture(0);


    //set the format into MJPG in the FourCC format 
    //cap0.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M','J','P','G'));
    cap0.set(CV_CAP_PROP_FPS, 15.0f);

    cap0.set(CAP_PROP_FRAME_WIDTH, 640);
    cap0.set(CAP_PROP_FRAME_HEIGHT, 360);


    VideoCapture cap2 = VideoCapture(2);
    //cap2.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M','J','P','G'));
    cap2.set(CV_CAP_PROP_FPS, 15.0f);

    cap2.set(CAP_PROP_FRAME_WIDTH, 640);
    cap2.set(CAP_PROP_FRAME_HEIGHT, 360);




    Mat frameA;
    if (cap0.isOpened()) cap0>>frameA;
    cv::imwrite("source1.png", frameA);        

    Mat frameB;
    if (cap2.isOpened()) cap2>>frameB;
    cv::imwrite("source2.png", frameB);





    return 0;
}

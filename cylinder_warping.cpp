#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include<unistd.h>
#include<zmq.hpp>


int main(){


    cv::FileStorage fs("camera1.yml",cv::FileStorage::READ);

    cv::Point2d fov;
    cv::Mat distCoeffs;
    cv::Mat cameraMatrix;

    if(fs.isOpened()){
  
        	
        fs["camera coeffs"] >> distCoeffs;
        fs["camera matrix"] >> cameraMatrix;
        fs["camera FOV"] >> fov;

    }
    else{
        std::cerr<<"the camera report error"<<std::endl;
        return 0;
    }   

    std::cout<<distCoeffs<<std::endl;
    std::cout<<cameraMatrix<<std::endl;
    std::cout<<fov<<std::endl;

    cv::Mat image;
    cv::Mat UndistortImage;


    cv::VideoCapture capture(0);
    
    sleep(1);
    capture >> image;
    int width;
    int height;

    height = image.rows;
    width = image.cols;
    
    cv::Mat map1, map2;
    cv::Size imageSize(width, height);

 
    cv::imwrite("source.png", image); 
    undistort(image, UndistortImage, cameraMatrix, distCoeffs);
    cv::imwrite("test.png", UndistortImage);


    // video transport

    std::string IP="192.168.137.1";
    /* 实例化用来发送帧的zmq对象 */
    zmq::context_t context(1);
    /* zmq对象使用TCP通讯协议 */
    zmq::socket_t footage_socket(context, ZMQ_PAIR);
    /* zmq对象和视频接收端建立TCP通讯协议*/
    footage_socket.connect("tcp://" + IP + ":5555");


    cv::Mat frame;
    while(true)
    {
        capture >> frame; // 采集图像一帧到frame

	// Encode the frame as JPEG
        std::vector<uchar> buffer; 
        std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 80};
        cv::imencode(".jpg", frame, buffer, params);

	// Send the encoded frame to PC
        zmq::message_t request(buffer.size());
        memcpy(request.data(), buffer.data(),buffer.size());
	footage_socket.send(request);

	cv::waitKey(10);

    }




    return 0;
}

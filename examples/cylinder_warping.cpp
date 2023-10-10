#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include<unistd.h>
#include<zmq.hpp>
#include<chrono>

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
    capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);   
    capture.set(cv::CAP_PROP_FPS, 60);



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

    std::chrono::high_resolution_clock clock;
    auto startTime = clock.now();
    int frameCount = 0;
    double fps;


    cv::Mat frame;
    while(true)
    {

        // 采集图像一帧到frame
	    capture >> frame;
	    // Encode the frame as JPEG
        // std::vector<uchar> buffer; 
        // std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 80};
        // cv::imencode(".jpg", frame, buffer, params);

	    // // Send the encoded frame to PC
        // zmq::message_t request(buffer.size());
        // memcpy(request.data(), buffer.data(),buffer.size());
	    // footage_socket.send(request);

        frameCount++;
        auto endTime = clock.now();
        //计算两个时间点之间时间差（以秒为单位）
        std::chrono::duration<double> elapsedTime = endTime - startTime;
        
        if (elapsedTime.count() >= 1.0) {
            // 计算帧率
            fps = frameCount / elapsedTime.count();
            
            // 打印帧率
            std::cout << "FPS: " << fps << std::endl;
            
            // 重置计时器和帧数计数器
            startTime = endTime;
            frameCount = 0;
    }


    }




    return 0;
}

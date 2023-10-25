#include <opencv2/opencv.hpp>
#include<zmq.hpp>
#include <chrono>
#include <thread>

int main()
{
    
    zmq::context_t context(1);
    zmq::socket_t footage_socket1(context, ZMQ_PAIR);
    zmq::socket_t footage_socket2(context, ZMQ_PAIR);

    footage_socket1.bind("tcp://*:5555");
    footage_socket2.bind("tcp://*:5556");

   // 绑定到指定的地址和端口
    while(true){

        std::cout <<"listen" << std::endl;
        //接受 TCP传输过来的一帧视频图像
        zmq::message_t frame1;
        footage_socket1.recv(&frame1);

        zmq::message_t frame2;
        footage_socket2.recv(&frame2);

        //接受数据转化为opencv的图像数据
        std::string frameData1(static_cast<char*>(frame1.data()), frame1.size());
        std::vector<uchar> imgData1(frameData1.begin(), frameData1.end());
        cv::Mat source1 = cv::imdecode(cv::Mat(imgData1), cv::IMREAD_COLOR);


        std::string frameData2(static_cast<char*>(frame2.data()), frame2.size());
        std::vector<uchar> imgData2(frameData2.begin(), frameData2.end());
        cv::Mat source2 = cv::imdecode(cv::Mat(imgData2), cv::IMREAD_COLOR);


        //显示图像
        cv::imshow("Stream1", source1);
        cv::imshow("Stream2", source2);        
        cv::waitKey(1);

    }

    return 0;    
}
#include "CameraStreamer.hpp"
#include "AudioStreamer.hpp"
#include <opencv2/opencv.hpp>
#include<zmq.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <portaudio.h>

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

	vector<int> audio_index;
	audio_index.push_back(1);

	//CameraStreamer cam(capture_index);
    AudioStreamer  as;
	// build a tunel for image transport

	


	return 0;

}





// #include <iostream>
// #include <fstream>
// #include <portaudio.h>

// // 配置录音参数
// #define CHUNK (1024)
// #define FORMAT (paInt16)
// #define CHANNELS (1)
// #define RATE (44100)
// #define RECORD_SECONDS (5)
// #define WAVE_OUTPUT_FILENAME "output.wav"

// int main()
// {
//     PaStreamParameters inputParameters;
//     PaStream* stream = nullptr;
//     std::ofstream outfile;

//     // 初始化PortAudio
//     PaError err = Pa_Initialize();
//     if (err != paNoError) {
//         std::cout << "PortAudio 初始化失败: " << Pa_GetErrorText(err) << std::endl;
//         return 1;
//     }

//     // 设置录音输入参数
//     inputParameters.device = Pa_GetDefaultInputDevice();
//     inputParameters.channelCount = CHANNELS;
//     inputParameters.sampleFormat = FORMAT;
//     inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighOutputLatency;
//     inputParameters.hostApiSpecificStreamInfo = nullptr;

//     // 打开流
//     err = Pa_OpenStream(&stream, &inputParameters, nullptr, RATE, CHUNK, paClipOff, nullptr, nullptr);
//     if (err != paNoError) {
//         std::cout << "打开流失败: " << Pa_GetErrorText(err) << std::endl;
//         Pa_Terminate();
//         return 1;
//     }

//     // 打开输出文件
//     outfile.open(WAVE_OUTPUT_FILENAME, std::ios::out | std::ios::binary);
//     if (!outfile.is_open()) {
//         std::cout << "打开输出文件失败" << std::endl;
//         Pa_CloseStream(stream);
//         Pa_Terminate();
//         return 1;
//     }

//     // 开始录音
//     std::cout << "开始录音..." << std::endl;
//     err = Pa_StartStream(stream);
//     if (err != paNoError) {
//         std::cout << "启动录音失败: " << Pa_GetErrorText(err) << std::endl;
//         outfile.close();
//         Pa_CloseStream(stream);
//         Pa_Terminate();
//         return 1;
//     }

//     int numSamples = static_cast<int>(RATE * RECORD_SECONDS);
//     short* samples = new short[CHUNK * CHANNELS];
//     int framesToRecord = numSamples / CHUNK;

//     for(int i = 0; i < framesToRecord; i++) {
//         err = Pa_ReadStream(stream, samples, CHUNK);
//         if (err != paNoError) {
//             std::cout << "录音过程中发生错误: " << Pa_GetErrorText(err) << std::endl;
//             delete[] samples;
//             outfile.close();
//             Pa_StopStream(stream);
//             Pa_CloseStream(stream);
//             Pa_Terminate();
//             return 1;
//         }
//         outfile.write(reinterpret_cast<const char*>(samples), CHUNK * sizeof(short) * CHANNELS);
//     }

//     std::cout << "录音完成." << std::endl;

//     // 停止录音并关闭流
//     err = Pa_StopStream(stream);
//     if (err != paNoError) {
//         std::cout << "停止录音失败: " << Pa_GetErrorText(err) << std::endl;
//     }
//     Pa_CloseStream(stream);

//     // 关闭输出文件
//     outfile.close();

//     // 清理资源
//     delete[] samples;
//     Pa_Terminate();

//     return 0;
// }

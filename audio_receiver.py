import zmq
import pyaudio
import numpy as np
import cv2
from receiver.audio_tools import Audio_zmq, ArrayVisualizer
from receiver.image_tools import Image_zmq, ImageVisualizer

# 初始化PyAudio
# pa = pyaudio.PyAudio()
# FORMAT = pyaudio.paFloat32
# CHANNELS = 1
# RATE = 16000
# CHUNK = 1280

# 创建PyAudio输出流
# stream = pa.open(format=FORMAT,
#                  channels=CHANNELS,
#                  rate=RATE,
#                  output=True,
#                  frames_per_buffer=CHUNK)



# 初始化ZeroMQ上下文和套接字


au = Audio_zmq() 
au_vis = ArrayVisualizer(100, 1280) 


im_capture = Image_zmq()
im_capture.start()

try:
    print("开始接收声音...")
    while True:
        # 从ZeroMQ套接字接收音频数据
        # audio_data = au.receive_data()
        # print("read the audio successfully")
        # # 将音频数据写入输出流进行播放
        # au_vis.update(audio_data)

        image_data = im_capture.get_latest_image()
        if image_data is not None:
            print("read the image successfully")
            cv2.imshow('Stream1', image_data)
            cv2.waitKey(10)
        else:
            print("waiting for conntecting")
            cv2.waitKey(100)

except KeyboardInterrupt:
    im_capture.stop()
    print("here")
  







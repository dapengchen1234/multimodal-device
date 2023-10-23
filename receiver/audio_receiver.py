import zmq
import pyaudio
from audio_tools import Audio_zmq, ArrayVisualizer


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

try:
    print("开始接收声音...")
    while True:
        # 从ZeroMQ套接字接收音频数据
        data = au.receive_data()
        print("read the audio successfully")
        # 将音频数据写入输出流进行播放
        au_vis.update(data)


except KeyboardInterrupt:
    pass


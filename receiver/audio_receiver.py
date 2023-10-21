import zmq
import pyaudio

# 初始化PyAudio
pa = pyaudio.PyAudio()
FORMAT = pyaudio.paFloat32
CHANNELS = 1
RATE = 16000
CHUNK = 1280

# 创建PyAudio输出流
stream = pa.open(format=FORMAT,
                 channels=CHANNELS,
                 rate=RATE,
                 output=True,
                 frames_per_buffer=CHUNK)

# 初始化ZeroMQ上下文和套接字
context = zmq.Context()
socket = context.socket(zmq.PULL)
socket.bind("tcp://192.168.137.1:4444")

try:
    print("开始接收声音...")
    while True:
        # 从ZeroMQ套接字接收音频数据
        data = socket.recv()
        # 将音频数据写入输出流进行播放
        print("read the audio successfully")
        stream.write(data)
except KeyboardInterrupt:
    pass
finally:
    # 关闭输出流和ZeroMQ套接字
    stream.stop_stream()
    stream.close()
    socket.close()
    pa.terminate()

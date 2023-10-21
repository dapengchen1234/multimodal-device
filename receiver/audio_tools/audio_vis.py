import pyaudio
import numpy as np
import matplotlib.pyplot as plt

class AudioVisualizer:
    def __init__(self):
        self.CHUNK = 1024  # 每个缓冲区的大小
        self.FORMAT = pyaudio.paInt16  # 采样格式为16位整数
        self.CHANNELS = 1  # 单声道
        self.RATE = 44100  # 采样率为44100Hz
        self.p = pyaudio.PyAudio()
        
        self.stream = self.p.open(format=self.FORMAT,
                                  channels=self.CHANNELS,
                                  rate=self.RATE,
                                  input=True,
                                  frames_per_buffer=self.CHUNK)
        
        self.fig, self.ax = plt.subplots()
        self.x = np.arange(0, 2 * self.CHUNK, 2)
        self.line, = self.ax.plot(self.x, np.random.rand(self.CHUNK), '-', lw=2)

        self.ax.set_xlim(0, 2 * self.CHUNK)
        self.ax.set_ylim(-32768, 32767)
        
        plt.show(block=False)
    
    def visualize_audio(self):
        while True:
            data = self.stream.read(self.CHUNK)
            data_np = np.frombuffer(data, dtype=np.int16)
            self.line.set_ydata(data_np)
            plt.draw()
            plt.pause(0.01)
    
    def close(self):
        self.stream.stop_stream()
        self.stream.close()
        self.p.terminate()

# Create an instance of the AudioVisualizer class
visualizer = AudioVisualizer()

# Start visualizing the audio
visualizer.visualize_audio()

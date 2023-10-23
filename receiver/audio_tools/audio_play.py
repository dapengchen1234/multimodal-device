import pyaudio
import numpy as np

class RealtimeAudioPlayer:
    def __init__(self, chunk_size=1024, sample_rate=44100):
        self.chunk_size = chunk_size
        self.sample_rate = sample_rate
        self.audio_stream = pyaudio.PyAudio().open(format=pyaudio.paFloat32,
                                                   channels=1,
                                                   rate=self.sample_rate,
                                                   output=True,
                                                   frames_per_buffer=self.chunk_size)

    def play(self):
        while True:
            data = np.random.randn(self.chunk_size)
            self.audio_stream.write(data.astype(np.float32).tostring())

    def stop(self):
        self.audio_stream.stop_stream()
        self.audio_stream.close()
        pyaudio.PyAudio().terminate()

# 使用示例
player = RealtimeAudioPlayer()
player.play()  # 开始播放实时声音

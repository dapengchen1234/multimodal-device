import zmq
import numpy as np

class Audio_zmq:
    def __init__(self, IP=None):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PULL)
        self.ip = "tcp://192.168.137.1:4444" if IP is None else IP

        # 初始化 socket 

        self.socket.bind(self.ip)

    def __del__(self):
        self.close()

    def send_message(self, message):
        pass 

    def receive_message(self):
        ## 返回原始数据
        return self.socket.recv()
    

    def receive_data(self):
        data = self.receive_message()
        data_np = np.frombuffer(data, dtype=np.float32)
        return data_np
        

    def close(self):
        self.socket.close()
        self.context.term()

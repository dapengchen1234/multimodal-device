import zmq 
import cv2
import numpy as np

class Image_zmq:

    def __init__(self, IP=None):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PULL)
        self.ip = "tcp://192.168.137.1:5555" if IP is None else IP

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
        data_np = np.frombuffer(data, dytpe=np.float32)
        img = cv2.imdecode(data_np, cv2.IMREAD_COLOR)
        return img
    

    def close(self):
        self.socket.close()
        self.context.term()



import zmq 
import cv2
import numpy as np
import threading

class Image_zmq:

    def __init__(self, IP=None):

        # 初始化 socket
        self.ip = "tcp://192.168.137.1:5555" if IP is None else IP

        # 用一个线程来接受图片
        self.receiver_thread = threading.Thread(target=self._receive_image)

        # image position 
        self.lastest_image_data = None 

        #
        self.lock = threading.Lock()
        
        self.stop_event = threading.Event()



    def _receive_image(self):
        ## 返回原始数据
        context = zmq.Context()
        socket = context.socket(zmq.PULL)  
        socket.bind(self.ip)
  

        while not self.stop_event.is_set():  # 检查停止标志位

            data = socket.recv()
            data_np = np.frombuffer(data, np.uint8)
            img = cv2.imdecode(data_np, cv2.IMREAD_COLOR)
            with self.lock:
                self.lastest_image_data = img           
            cv2.waitKey(10)

        socket.close()
        context.term()

    
    def start(self):
        self.receiver_thread.start()
    
    def stop(self):
        print("Loop stopped by users")

        self.stop_event.set()
        self.receiver_thread.join()


    def get_latest_image(self):
        with self.lock:
            return self.lastest_image_data
    



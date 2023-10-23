import cv2
import zmq
import numpy as np

# context = zmq.Context()
# footage_socket1 = context.socket(zmq.PULL)
# footage_socket1.bind("tcp://192.168.137.1:5555")

# footage_socket2 = context.socket(zmq.PULL)
# footage_socket2.bind("tcp://192.168.137.1:5556")

# print("here")
# while True:
#     try:
#         # Receive frame 1
#         frame_data1 = footage_socket1.recv()
#         nparr1 = np.frombuffer(frame_data1, np.uint8)
#         source1 = cv2.imdecode(nparr1, cv2.IMREAD_COLOR)

#         # Receive frame 2

#         # frame_data2 = footage_socket2.recv()
#         # nparr2 = np.frombuffer(frame_data2, np.uint8)
#         # source2 = cv2.imdecode(nparr2, cv2.IMREAD_COLOR)

#         # Display frames
#         cv2.imshow('Stream1', source1)

#         # cv2.imshow('Stream2', source2)
#         cv2.waitKey(10)

#         print("Success")

#     except zmq.error.ZMQError as e:
#         print(f"Exception: {e}")

# cv2.destroyAllWindows()


import tkinter as tk

def create_window(window_number):
    # 创建新窗口
    window = tk.Tk()
    window.title("Window {}".format(window_number))
    
    # 在窗口中添加组件或执行其他操作
    label = tk.Label(window, text="This is Window {}".format(window_number))
    label.pack()
    
    # 运行窗口的主循环
    window.mainloop()

# 设置N个窗口
N = 5

# 循环创建并运行N个窗口
for i in range(N):
    create_window(i+1)

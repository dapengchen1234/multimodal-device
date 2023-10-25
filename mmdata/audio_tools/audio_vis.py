import matplotlib.pyplot as plt
from collections import deque

class ArrayVisualizer:
    def __init__(self, length, sample_frame):
        self.length = length
        self.frame = sample_frame
        self.total = self.length*self.frame 

      
        self.fig, self.ax = plt.subplots()
        self.ax.set_xlim(0,  self.total-1)
        self.ax.set_ylim(-1, 1)
        self.line, = self.ax.plot(range( self.total), [0] *   self.total)

        ## 初始化array
        self.array = []
        for i in range(self.length):
            self.array.append([0]*self.frame)

        # print(len(self.array))
        # self.array.pop(0) 
        # print(len(self.array))


    def update(self, new_data):
 
        
        self.array.append(new_data)
        if len(self.array) > self.length:
            self.array.pop(0)        


        merged_array = [element for sub_array in self.array for element in sub_array]
        self.line.set_ydata(merged_array)
        plt.pause(0.001)


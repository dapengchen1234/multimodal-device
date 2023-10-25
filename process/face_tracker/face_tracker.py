import cv2 
import mediapipe as mp 
import numpy as np
import time

class FaceTrack():

    def __init__(self, args):

        self.w = 640
        self.h = 480
        self.face_threshold = 0.65
        self.frame_index = 1
        self.track_frace = {}
        self.args = args 

        # init mediapipe detector
        mp_face_detection = mp.solutions.face_detection 
        self.detector = mp_face_detection.FaceDetection(model_selection=1, min_detection_confidence=0.2)
        print("mediapipe init success")

        # init tracker
        


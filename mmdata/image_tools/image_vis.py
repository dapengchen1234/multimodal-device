import cv2
import numpy as np

class ImageVisualizer:
    def __init__(self, num_videos, num_perrow):
        self.num_videos = num_videos
        self.num_perrow = num_perrow

        # Create the visualization window
        cv2.namedWindow("Multi-Video Stream")

    def __del__(self):
        cv2.destroyAllWindows()


    def update(self, video_frames):
        rows = []

        # Calculate the number of remaining frames
        remaining_frames = len(video_frames) % self.num_perrow

        for i in range(0, len(video_frames) - remaining_frames, self.num_perrow):
            row = video_frames[i:i + self.num_perrow]
            rows.append(row)

        # Handle remaining frames
        if remaining_frames > 0:
            # Append blank frames to the last row
            last_row = video_frames[-remaining_frames:] + [np.zeros_like(video_frames[0])] * (self.num_perrow - remaining_frames)
            rows.append(last_row)

        # Rest of the code...

        # Resize frames and concatenate rows horizontally
        resized_rows = [np.concatenate([cv2.resize(frame, (320, 240)) for frame in row], axis=1) for row in rows]

        # Concatenate rows vertically
        vis = np.concatenate(resized_rows, axis=0)

        # Display the final visualization
        cv2.imshow("Multi-Video Stream", vis)

        cv2.waitKey(1)

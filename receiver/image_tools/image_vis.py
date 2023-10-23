import cv2
import numpy as np

class MultiVideoStreamVisualizer:
    def __init__(self, num_videos):
        self.num_videos = num_videos

        # Create the visualization window
        cv2.namedWindow("Multi-Video Stream")

    def play(self, frames):
        while True:
            video_frames = []
            for i in range(self.num_videos):
                frame = frames[i]
                if frame is None:
                    break
                video_frames.append(frame)

            rows = []
            for i in range(0, len(video_frames), 3):
                row = video_frames[i:i + 3]
                rows.append(row)

            if len(rows) > 0:
                # Resize frames and concatenate rows horizontally
                resized_rows = [np.concatenate([cv2.resize(frame, (320, 240)) for frame in row], axis=1) for row in rows]

                # Concatenate rows vertically
                vis = np.concatenate(resized_rows, axis=0)

                # Display the final visualization
                cv2.imshow("Multi-Video Stream", vis)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        cv2.destroyAllWindows()

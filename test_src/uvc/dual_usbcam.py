import cv2

def main():
    # Initialize the first USB camera
    cap1 = cv2.VideoCapture(0)

    # Initialize the second USB camera
    cap2 = cv2.VideoCapture(2)
    
    # Initialize the second USB camera
    cap3 = cv2.VideoCapture(4)

    while True:
        # Capture frame from first camera
        ret1, frame1 = cap1.read()

        # Capture frame from second camera
        ret2, frame2 = cap2.read()

        # Capture frame from second camera
        ret3, frame3 = cap3.read()
        
        # Check if frames are captured successfully
        if not ret1:
            print("Failed to capture frame from Camera 1")
            break
        if not ret2:
            print("Failed to capture frame from Camera 2")
            break

        # Resize frames to have same height for concatenation
        height = min(frame1.shape[0], frame2.shape[0], frame3.shape[0])
        frame1 = cv2.resize(frame1, (int(frame1.shape[1]*height/frame1.shape[0]), height))
        frame2 = cv2.resize(frame2, (int(frame2.shape[1]*height/frame2.shape[0]), height))
        frame3 = cv2.resize(frame3, (int(frame3.shape[1]*height/frame3.shape[0]), height))

        # Display the resulting frames side by side
        combined_frame = cv2.hconcat([frame1, frame2, frame3])
        cv2.imshow('Triple USB Cameras by JWT using python', combined_frame)

        # Press 'q' to exit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the captures
    cap1.release()
    cap2.release()
    cap3.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()


import cv2

def main():
    # Initialize video capture with the first UVC camera
    cap = cv2.VideoCapture(0)

    # Attempt to reduce buffering; not all backends support this property
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

    if not cap.isOpened():
        print("Error: Could not open video device.")
        return

    print("Press 'q' to quit.")
    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()
        if not ret:
            print("Error: Unable to read from the camera.")
            break

        # Display the resulting frame
        cv2.imshow('UVC Camera', frame)

        # Break the loop when 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # When everything is done, release the capture
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()

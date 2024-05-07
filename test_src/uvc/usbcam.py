import cv2

def main():
    # Initialize the USB camera
    cap = cv2.VideoCapture(0)  # Use '0' for the first camera device, '1' for the second, and so on

    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()

        # Display the resulting frame
        cv2.imshow('USB Camera Feed', frame)

        # Press 'q' to exit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the capture
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()


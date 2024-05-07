#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
    // Use the first video capture device on the system
    cv::VideoCapture capture(0);

    if (!capture.isOpened()) {
        std::cerr << "ERROR: Could not open video device" << std::endl;
        return 1;
    }

    // Set capture properties, if needed
    // capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    // capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    // capture.set(cv::CAP_PROP_FPS, 30);

    cv::Mat frame;

    // Create a window to display the frames
    cv::namedWindow("UVC Camera", cv::WINDOW_NORMAL);

    std::cout << "Press any key to exit" << std::endl;

    // Main loop to capture and display frames
    while (true) {
        capture >> frame; // Capture a frame

        // Check if the frame is empty
        if (frame.empty()) {
            std::cerr << "ERROR: Frame is empty" << std::endl;
            break;
        }

        cv::imshow("UVC Camera", frame); // Display the frame

        // Exit loop if any key is pressed
        if (cv::waitKey(5) >= 0) break;
    }

    // Release the video capture object
    capture.release();

    // Close all OpenCV windows
    cv::destroyAllWindows();

    return 0;
}

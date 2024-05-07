// sudo apt-get install libopencv-dev
// g++ NobufferingUVC.c -o NoBufUVC $(pkg-config --cflags --libs opencv4) -std=c++11

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    int fd;
    fd = open("/dev/video0", O_RDWR);
    if (fd == -1) {
        perror("Opening video device");
        return 1;
    }

    struct v4l2_capability cap;
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        perror("Querying Capabilities");
        return 1;
    }

    struct v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    format.fmt.pix.width = 640;
    format.fmt.pix.height = 480;

    if (ioctl(fd, VIDIOC_S_FMT, &format) == -1) {
        perror("Setting Pixel Format");
        return 1;
    }

    struct v4l2_requestbuffers req;
    req.count = 2; // Requesting 2 buffers for minimal buffering
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
        perror("Requesting Buffer");
        return 1;
    }

    printf("Streaming started.\n");
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1) {
        perror("Start Capture");
        return 1;
    }

    struct v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
        perror("Query Buffer");
        return 1;
    }

    if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
        perror("Retrieve Frame");
        return 1;
    }

    // Assuming the frame is in MJPEG format, which is common for UVC cameras.
    // You might need to convert it to a format OpenCV can display (e.g., YUYV to BGR).
    // This step is skipped here for brevity.

    printf("Frame captured.\n");

    // Here you would convert and display the frame using OpenCV.
    // This example does not include the conversion code for simplicity.

    // Stop streaming
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMOFF, &type) == -1) {
        perror("Stop Capture");
        return 1;
    }

    close(fd);
    return 0;
}

#ifndef CAMERA_THREAD_H
#define CAMERA_THREAD_H

#include <QThread>
#include <QMutex>
#include <QDebug>
#include <opencv4/opencv2/opencv.hpp>

class camera_thread : public QThread
{
public:
    explicit camera_thread(cv::VideoCapture *videocap);
    cv::Mat getFrame();
    cv::VideoCapture *cap;
    cv::Mat frame;
private:
    bool UpdateFlag;

protected:
    void run() override;
};

#endif // CAMERA_THREAD_H

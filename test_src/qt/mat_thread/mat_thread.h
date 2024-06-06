#ifndef MAT_THREAD_H
#define MAT_THREAD_H
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <opencv4/opencv2/opencv.hpp>

class mat_thread : public QThread
{
public:
    explicit mat_thread(cv::Mat *frame);
    cv::Mat *frame;
protected:
    void run() override;
};

#endif // MAT_THREAD_H

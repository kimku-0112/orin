#include "mat_thread.h"


mat_thread::mat_thread(cv::Mat *frame)
{
    this->frame = frame;
}

void mat_thread::run(){
    while(1){
        cv::putText(*frame, "test string", cv::Point(0, 50), 1, 2, cv::Scalar(255, 255, 0), 1, 8);
        msleep(20);
    }
}

#include "camera_thread.h"

camera_thread::camera_thread(cv::VideoCapture *videocap)
{
    this->cap = videocap;
    this->UpdateFlag = true;
}

void camera_thread::run(){
    while(1){
        if(UpdateFlag){
            if(cap->read(frame)){
                UpdateFlag = false;
            }
        }
        msleep(20);
    }
}
cv::Mat camera_thread::getFrame(){
    if(!UpdateFlag){
        UpdateFlag = true;
        return this->frame;
        qDebug() << "Error";
    }
    return cv::Mat();
}

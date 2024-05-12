#include "camera_thread.h"

camera_thread::camera_thread(cv::VideoCapture *videocap)
{
    this->cap = videocap;
    this->UpdateFlag = true;
}

void camera_thread::run(){
    while(1){
        if(UpdateFlag){
            UpdateFlag = false;
            *cap>>frame;
        }
        msleep(2);
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

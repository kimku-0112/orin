#include "camera_open.h"
#include <QDebug>

void camera_open(cv::VideoCapture *cap,QLabel *Camera,int width,int height){

    if (!cap->isOpened()){
        Camera->setText("Error: Could not open camera");
        return;
    }

    cv::Mat frame;
    *cap >> frame;
    if(cap->get(cv::CAP_PROP_FRAME_WIDTH) > width){
//        qDebug() << "camera resize";
        cv::resize(frame, frame, cv::Size(width, height));
    }
    if (!frame.empty()) {
        // OpenCV Mat을 Qt QImage로 변환
        QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);

        Camera->setPixmap(QPixmap::fromImage(qimg));
    }
}

#ifndef CAMERA_OPEN_H
#define CAMERA_OPEN_H

#include <QLabel>
#include "opencv2/opencv.hpp"

void camera_open(cv::VideoCapture *cap,QLabel *Camera,int width,int height);

#endif // CAMERA_OPEN_H

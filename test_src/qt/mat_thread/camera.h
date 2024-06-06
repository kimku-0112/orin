#ifndef CAMERA_H
#define CAMERA_H
#include <opencv4/opencv2/opencv.hpp>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>

void CaptureOpenVideoFile(QWidget *parent, cv::VideoCapture *cap);



#endif // CAMERA_H

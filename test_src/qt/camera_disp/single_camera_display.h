#ifndef SINGLE_CAMERA_DISPLAY_H
#define SINGLE_CAMERA_DISPLAY_H

#include <QWidget>
#include <QTimer>
#include "camera_open.h"
#include "opencv2/opencv.hpp"

namespace Ui {
class single_camera_display;
}

class single_camera_display : public QWidget
{
    Q_OBJECT

public:
    explicit single_camera_display(QWidget *parent = nullptr);
    ~single_camera_display();
    void updateFrame(int width, int height);
    cv::VideoCapture *cap;

    void getCameraLabelSize(int &width,int &height);

private:
    Ui::single_camera_display *ui;
};

#endif // SINGLE_CAMERA_DISPLAY_H

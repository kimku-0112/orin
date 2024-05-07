#ifndef DUAL_CAMERA_DISPLAY_H
#define DUAL_CAMERA_DISPLAY_H

#include <QWidget>
#include <QTimer>
#include "camera_open.h"
#include "opencv2/opencv.hpp"

namespace Ui {
class dual_camera_display;
}

class dual_camera_display : public QWidget
{
    Q_OBJECT

public:
    explicit dual_camera_display(QWidget *parent = nullptr);
    ~dual_camera_display();
    void updateFrame(int width,int height);

    cv::VideoCapture *cap[2];

    void getCameraLabelSize(int &width,int &height);



private:
    Ui::dual_camera_display *ui;
};

#endif // DUAL_CAMERA_DISPLAY_H

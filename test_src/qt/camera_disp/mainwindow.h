#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QByteArray>
#include <cstring>
#include "dual_camera_display.h"
#include "single_camera_display.h"
#include "opencv2/opencv.hpp"
#include "camera_open.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    dual_camera_display *dual_camera_widget;
    single_camera_display *single_camera_widget;

    cv::VideoCapture *cap[2];

    QTimer *timer;

    void updateFrame();

    void putUARTReceive(char*data,int len);
    void putCANReceive(char*data,int len);



private slots:
    void on_single_camera_btn_clicked();

    void on_dual_camera_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

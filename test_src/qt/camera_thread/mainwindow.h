#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>

#include "camera_thread.h"
#include "camera.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    camera_thread *CameraThread1;
    camera_thread *CameraThread2;
    camera_thread *CameraThread3;

    cv::VideoCapture cap1;
    cv::VideoCapture cap2;
    cv::VideoCapture cap3;

    QTimer *Timer1;
    QTimer *Timer2;
    QTimer *Timer3;

    int videoFPS1;
    int videoFPS2;
    int videoFPS3;
    int startMilliSec;

    void UpdateFrame1();
    void UpdateFrame2();
    void UpdateFrame3();
    bool frameUpdate(camera_thread *Thread,cv::Mat *frame,QLabel *label,const char* str);


private slots:
    void on_pushButton_openvideofile_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

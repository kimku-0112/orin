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
#include "mat_thread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    camera_thread *CameraThread;
    mat_thread *MatThread;

    cv::VideoCapture cap;
    cv::Mat frame;

    QTimer *Timer;

    void UpdateFrame();
    bool frameUpdate(camera_thread *Thread,cv::Mat *frame,QLabel *label);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

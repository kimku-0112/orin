#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include <QTreeWidget>
#include <QTimer>
#include <QDateTime>
#include <QTextBrowser>
#include <QMutex>
#include <opencv4/opencv2/opencv.hpp>
#include "comm_thread.h"

QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QWidget *MainWidget;
    QWidget *MenuDialog;

    struct termios uart_config;
    int uart_port;

    struct can_frame canframe;
    int can_sock;

    UartReadThread *UReadThread;
    UartWriteThread *UWriteThread;
    CanReadThread *CReadThread;
    CanWriteThread *CWriteThread;

    QMutex mutexuart;
    QMutex mutexcan;


public slots:
    void slot_M_input();
    void slot_S_input();
    void slot_Return_input();
    void updateFrame1();
    void updateFrame2();
    void updateFrame3();
    void startRecording();
    void stopRecording();
    void stopAndStartRecording();

signals:
    void key_M_Pushed();
    void key_Up_Pushed();
    void key_Down_Pushed();
    void key_Left_Pushed();
    void key_Right_Pushed();
    void key_Return_Pushed();
    void key_S_Pushed();

private:
    // Menu
    QPushButton * MenuBtn;   
    // Video
    cv::VideoCapture cap1;
    cv::VideoCapture cap2;
    cv::VideoCapture cap3;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QTreeWidget* treewidget;
    QLabel * GunSelect;
    QTimer *timer1;
    QTimer *timer2;
    QTimer *timer3;
    QTimer *singleShot;
    bool isRecording;
    bool isHourlyRecording;
    cv::VideoWriter writer;
    QTextBrowser* textBrowser;
    //Ui::MainWindow *ui;
protected:
    void keyReleaseEvent(QKeyEvent * event);
    void appendToLog(const QString &text);
};
#endif // MAINWINDOW_H

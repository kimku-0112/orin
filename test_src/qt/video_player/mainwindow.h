#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTimer *timer;
    void updateFrame();

private slots:
    void on_openButton_clicked();
    void on_PlayButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture videoCapture;
};

#endif // MAINWINDOW_H

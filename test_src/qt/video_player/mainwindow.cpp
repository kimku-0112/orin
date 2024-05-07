#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Video File"), "", tr("Video Files (*.mp4 *.avi *.mov)"));
    if (!filePath.isEmpty()) {
        videoCapture.open(filePath.toStdString());
        if (!videoCapture.isOpened()) {
            QMessageBox::warning(this, tr("Error"), tr("Could not open video file."));
        }
    }

    if (!videoCapture.isOpened()) {
        QMessageBox::warning(this, tr("Error"), tr("No video file opened."));
    }

}


void MainWindow::on_PlayButton_clicked()
{
    timer->start(30);
}

void MainWindow::updateFrame(){
    cv::Mat frame;
    if(videoCapture.read(frame)){
        int width = ui->video_label->width();
        cv::resize(frame,frame,cv::Size(width,width/16*9));
        QImage qimg(frame.data, frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
        ui->video_label->setPixmap(QPixmap::fromImage(qimg));

    }
    else{
        timer->stop();
    }
}

void MainWindow::on_stopButton_clicked()
{
    timer->stop();
}


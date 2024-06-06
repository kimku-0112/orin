#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CaptureOpenVideoFile(this,&cap);

    CameraThread = new camera_thread(&cap);

    CameraThread->start(QThread::LowPriority);

    MatThread = new mat_thread(&frame);

    MatThread->start(QThread::LowPriority);

    Timer = new QTimer(this);

    connect(Timer, &QTimer::timeout, this, &MainWindow::UpdateFrame);

    Timer->start(33);
}

void MainWindow::UpdateFrame(){
    frameUpdate(CameraThread, &frame, ui->label_camera1);
}

bool MainWindow::frameUpdate(camera_thread *Thread,cv::Mat *frame,QLabel *label){
    *frame = Thread->getFrame();
    if(!frame->empty()){
        int width = label->width();
        cv::resize(*frame,*frame,cv::Size(width,width/16*9));
        QImage qimg(frame->data,frame->cols,frame->rows,frame->step,QImage::Format_BGR888);
        label->setPixmap(QPixmap::fromImage(qimg));
        return true;
    }
    return false;
}

MainWindow::~MainWindow()
{
    delete ui;
}



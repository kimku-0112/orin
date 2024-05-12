#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoFPS1 = 0;
    videoFPS2 = 0;
    videoFPS3 = 0;

    if(!cap1.open(0)){
        qDebug() << "cap1 open Error!";
    }
    qDebug() << "cap1 open!";
    cap1.set(cv::CAP_PROP_FPS,30);
    qDebug() << "FPS: " << cap1.get(cv::CAP_PROP_FPS);
    qDebug() << "height: " << cap1.get(cv::CAP_PROP_FRAME_HEIGHT);
    qDebug() << "width: " << cap1.get(cv::CAP_PROP_FRAME_WIDTH);


    if(!cap2.open(2)){
        qDebug() << "cap2 open Error!";
    }

    qDebug() << "cap2 open!";
    cap2.set(cv::CAP_PROP_FPS,30);
    qDebug() << cap2.get(cv::CAP_PROP_FPS);
    qDebug() << "height: " << cap2.get(cv::CAP_PROP_FRAME_HEIGHT);
    qDebug() << "width: " << cap2.get(cv::CAP_PROP_FRAME_WIDTH);

    CaptureOpenVideoFile(this,&cap3);
    CameraThread1 = new camera_thread(&cap1);
    CameraThread2 = new camera_thread(&cap2);
    CameraThread3 = new camera_thread(&cap3);

    CameraThread1->start(QThread::LowPriority);
    CameraThread2->start(QThread::LowPriority);
    CameraThread3->start(QThread::LowPriority);

    Timer1 = new QTimer(this);
    Timer2 = new QTimer(this);
    Timer3 = new QTimer(this);

    connect(Timer1, &QTimer::timeout, this, &MainWindow::UpdateFrame1);
    connect(Timer2, &QTimer::timeout, this, &MainWindow::UpdateFrame2);
    connect(Timer3, &QTimer::timeout, this, &MainWindow::UpdateFrame3);

    startMilliSec = QDateTime::currentMSecsSinceEpoch();
    Timer1->start(33);
    Timer2->start(33);
    Timer3->start(41);
}

void MainWindow::UpdateFrame1(){
    cv::Mat frame;
    int time = QDateTime::currentMSecsSinceEpoch();
    time -= startMilliSec;
    videoFPS1 += frameUpdate(CameraThread1, &frame, ui->label_camera1,
                             QString("cap2 frame count: %2 / FPS: %1")
                             .arg(videoFPS1*1000/time).arg(videoFPS1).toUtf8().constData());

    ui->label_current_ms->setText(QString("Current ms : %1").arg(time));
}
void MainWindow::UpdateFrame2(){
    cv::Mat frame;
    int time = QDateTime::currentMSecsSinceEpoch();
    time -= startMilliSec;
    videoFPS2 += frameUpdate(CameraThread2, &frame, ui->label_camera2,
                             QString("cap2 frame count: %2 / FPS: %1")
                             .arg(videoFPS2*1000/time).arg(videoFPS2).toUtf8().constData());

}
void MainWindow::UpdateFrame3(){
    cv::Mat frame;
    int time = QDateTime::currentMSecsSinceEpoch();
    time -= startMilliSec;
    videoFPS3 += frameUpdate(CameraThread3, &frame, ui->label_camera3,
                             QString("cap3 frame count: %2 / FPS: %1")
                             .arg(videoFPS3*1000/time).arg(videoFPS3).toUtf8().constData());
}

bool MainWindow::frameUpdate(camera_thread *Thread,cv::Mat *frame,QLabel *label,const char* str){
    *frame = Thread->getFrame();
    if(!frame->empty()){
        int width = label->width();
        cv::resize(*frame,*frame,cv::Size(width,width/16*9));
        cv::putText(*frame, str, cv::Point(0, 50), 1, 2, cv::Scalar(255, 255, 0), 1, 8);
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


void MainWindow::on_pushButton_openvideofile_clicked()
{
    CaptureOpenVideoFile(this,&cap3);
}


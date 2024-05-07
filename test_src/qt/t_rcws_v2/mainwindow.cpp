#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv4/opencv2/opencv.hpp>
#include <QDebug>
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, ui(new Ui::MainWindow)
{
    //ui->setupUi(this);

    isRecording = false; // Recoriding flag
    isHourlyRecording = false;
    //=================================================
    // Camera Area
    //=================================================
    label1 = new QLabel(this);
    label2 = new QLabel(this);
    label3 = new QLabel(this);
    label1->setGeometry(0,40,1920,1080);
    label2->setGeometry(0,800,400,240);
    label3->setGeometry(0,560,400,240);

    cap1.open(0); // first cam
    cap2.open(2); // second cam
    cap3.open("/home/rcws/Downloads/testvideo.mp4"); // video
    //cap1.open("/home/rcws/Downloads/1.avi"); // first cam
    //cap2.open("/home/rcws/Downloads/2.avi"); // second cam
    //cap3.open("/home/rcws/Downloads/3.avi"); // video

    if(!cap1.isOpened() || !cap2.isOpened()) {
        qDebug() << "Error : Unable to connect to cameras";
    }
    if(!cap3.isOpened()) {
        qDebug() << "Error : Unable to connect to video";
    }

    timer1 = new QTimer(this);
    timer2 = new QTimer(this);
    timer3 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &MainWindow::updateFrame1);
    connect(timer2, &QTimer::timeout, this, &MainWindow::updateFrame2);
    connect(timer3, &QTimer::timeout, this, &MainWindow::updateFrame3);
    timer1->start(30);
    timer2->start(30);
    timer3->start(30);


    UART_Init(uart_config,uart_port);//uart
    CANFD_Init(canframe,can_sock);

    //add thread===================================
    UReadThread = new UartReadThread(&uart_config,&uart_port,&mutexuart,this);
    UWriteThread = new UartWriteThread(&uart_config,&uart_port,&mutexuart,this);
    CReadThread = new CanReadThread(&canframe,&can_sock,&mutexcan,this);
    CWriteThread = new CanWriteThread(&canframe,&can_sock,&mutexcan,this);

    //=============================================

    //add thread start=============================
    UReadThread->start();
    UWriteThread->start();
    CReadThread->start();
    CWriteThread->start();

    //=============================================


    //hourlyTimer = new QTimer(this);
    //connect(hourlyTimer, &QTimer::timeout, this, &MainWindow::saveHourlyVideo);
    //hourlyTimer->start(3600000); // 1시간마다 저장
    //hourlyTimer->start(60000); // 1분마다 저장
    //=================================================
    // Video Save Area
    //=================================================
    //connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::stopRecording);
    connect(this,SIGNAL(key_S_Pushed()), this, SLOT(slot_S_input()));
    connect(qApp, &QCoreApplication::aboutToQuit, [&]() {
            if (cap1.isOpened()) {
                cap1.release();
            }
        });
    //=================================================
    // MENU Area
    //=================================================
    QPalette p;
    p.setColor(QPalette::Window, QColor(45,55,65,255));
    setAutoFillBackground(true);
    setPalette(p);
    setGeometry(0,0,1920,1080);
    setWindowFlags(Qt::Window);
    //setWindowTitle("RCWS");
    MainWidget = new QWidget;
    MainWidget->setGeometry(0,0,1920,1080);
    setCentralWidget(MainWidget);

    //=================================================
    // Top Menu bar
    //=================================================
    QLabel * Top_bg_line = new QLabel(MainWidget);
    Top_bg_line->setStyleSheet("background-color: rgb(103,141,171)");
    Top_bg_line->setGeometry(0,0,1920,40);

    QLabel * GunTitle = new QLabel(MainWidget);
    GunTitle->move(50,10);
    GunTitle->setFont(QFont("Ubuntu regular",12));
    GunTitle->setText("화기 : ");
    GunTitle->setStyleSheet("color:rgb(226,226,226;");

    GunSelect = new QLabel(MainWidget);
    GunSelect->setText("K-16");
    GunSelect->setStyleSheet("background-color: rgba(0,0,0,255); color:rgb(226,226,226);border-color:rgb(100,100,100); border-radius:3px");
    GunSelect->setContentsMargins(8,0,8,0);
    GunSelect->setAlignment(Qt::AlignCenter);
    GunSelect->setGeometry(100,10,80,25);

    //=================================================
    // Bottom Menu bar
    //=================================================
    QLabel * bottom_bg_line = new QLabel(MainWidget);
    bottom_bg_line->setStyleSheet("background-color: rgb(50,0,50)");
    bottom_bg_line->setGeometry(0,1040,1920,40);
    MenuBtn = new QPushButton(MainWidget);
    MenuBtn->setGeometry(1840,1042,80,36);
    //MenuBtn->setGeometry(1220,500,80,36);
    MenuBtn->setText("메뉴");
    connect(this,SIGNAL(key_M_Pushed()),this,SLOT(slot_M_input()));
    MenuDialog = new QWidget(MainWidget);

    MenuDialog->setGeometry(1220,540,695,498);
    MenuDialog->setStyleSheet("background-color: rgb(255,255,255,30);border: 2px solid;border-color:rgb(100,100,100,100); border-radius:3px");
    MenuDialog->hide();

    treewidget = new QTreeWidget(MenuDialog);
    treewidget->setHeaderHidden(true);
    treewidget->setStyleSheet("background-color: rgb(255,255,255,30);border: 2px solid;border-color:rgb(100,100,100,100); border-radius:3px");

    QTreeWidgetItem* row1 = new QTreeWidgetItem;
    treewidget->addTopLevelItem(row1);
    row1->setText(0, "무장");
    QTreeWidgetItem* row1_1 = new QTreeWidgetItem;
    row1_1->setText(0, "화기");
    row1->addChild(row1_1);
    QTreeWidgetItem* row1_1_1 = new QTreeWidgetItem;
    row1_1_1->setText(0, "K-16");
    row1_1->addChild(row1_1_1);
    QTreeWidgetItem* row1_1_2 = new QTreeWidgetItem;
    row1_1_2->setText(0, "M60");
    row1_1->addChild(row1_1_2);
    QTreeWidgetItem* row1_2 = new QTreeWidgetItem;
    row1_2->setText(0, "탄환");
    row1->addChild(row1_2);
    QTreeWidgetItem* row1_2_1 = new QTreeWidgetItem;
    row1_2_1->setText(0, "장전수량");
    row1_2->addChild(row1_2_1);
    QTreeWidgetItem* row2 = new QTreeWidgetItem;
    treewidget->addTopLevelItem(row2);
    row2->setText(0, "영상");
    QTreeWidgetItem* row2_1 = new QTreeWidgetItem;
    row2_1->setText(0, "운용설정");
    row2->addChild(row2_1);
    QTreeWidgetItem* row2_1_1 = new QTreeWidgetItem;
    row2_1_1->setText(0, "디스플레이 밝기");
    row2_1->addChild(row2_1_1);
    QTreeWidgetItem* row2_1_2 = new QTreeWidgetItem;
    row2_1_2->setText(0, "동시송출");
    row2_1->addChild(row2_1_2);
    QTreeWidgetItem* row2_2 = new QTreeWidgetItem;
    row2_2->setText(0, "주간카메라");
    row2->addChild(row2_2);
    QTreeWidgetItem* row2_2_1 = new QTreeWidgetItem;
    row2_2_1->setText(0, "밝기");
    row2_2->addChild(row2_2_1);
    QTreeWidgetItem* row2_2_2 = new QTreeWidgetItem;
    row2_2_2->setText(0, "명암");
    row2_2->addChild(row2_2_2);
    QTreeWidgetItem* row2_2_3 = new QTreeWidgetItem;
    row2_2_3->setText(0, "떨림보정");
    row2_2->addChild(row2_2_3);
    QTreeWidgetItem* row2_2_4 = new QTreeWidgetItem;
    row2_2_4->setText(0, "샤프닝");
    row2_2->addChild(row2_2_4);
    QTreeWidgetItem* row2_2_5 = new QTreeWidgetItem;
    row2_2_5->setText(0, "잡음제거");
    row2_2->addChild(row2_2_5);
    QTreeWidgetItem* row2_2_6 = new QTreeWidgetItem;
    row2_2_6->setText(0, "역광보정");
    row2_2->addChild(row2_2_6);

    treewidget->setGeometry(5,5,200,488);
    //treewidget->setSortingEnabled(true);
    connect(this,SIGNAL(key_Return_Pushed()), this, SLOT(slot_Return_input()));

    // QTextBrowser
    textBrowser = new QTextBrowser(this);
    textBrowser->setGeometry(1520,40,400,200);
    textBrowser->setStyleSheet("background-color: rgb(255,255,255,120);border: 2px solid;border-color:rgb(100,100,100,100); border-radius:3px");
}


MainWindow::~MainWindow()
{
    //delete ui;
    delete label1;
    delete label2;
    delete label3;
    delete timer1;
    delete timer2;
    delete timer3;
    delete singleShot;

    UReadThread->quit();
    UWriteThread->quit();
    CReadThread->quit();
    CWriteThread->quit();

    UReadThread->wait();
    UWriteThread->wait();
    CReadThread->wait();
    CWriteThread->wait();
}

void MainWindow::updateFrame1()
{
    cv::Mat frame1;
    cap1 >> frame1;
    //cap2 >> frame2;
    //cap3 >> frame3;

//    int width = label3->width();
//    cv::resize(frame3,frame3,cv::Size(width,width/16*9));

    cv::Mat frame1_rgb, frame2_rgb;
    cv::cvtColor(frame1,frame1_rgb,cv::COLOR_BGR2RGB);
    //cv::cvtColor(frame2,frame2_rgb,cv::COLOR_BGR2GRAY);

    QImage img1(frame1_rgb.data, frame1_rgb.cols, frame1_rgb.rows, QImage::Format_RGB888);
    //QImage img2(frame2_rgb.data, frame2_rgb.cols, frame2_rgb.rows, QImage::Format_Indexed8);
    //QImage img3(frame3.data, frame3.cols, frame3.rows, QImage::Format_RGB888);

    label1->setPixmap(QPixmap::fromImage(img1));
    //label2->setPixmap(QPixmap::fromImage(img2));
    //label3->setPixmap(QPixmap::fromImage(img3));

    label1->setScaledContents(true);
    //label2->setScaledContents(true);
    //label3->setScaledContents(true);

//    // Recording
//    if(isRecording){
//        writer.write(frame1);
//    }



//    cv::Mat frame;
//    if(cap3.read(frame)){
//        int width = label1->width();
//        cv::resize(frame,frame,cv::Size(width,width/16*9));
//        QImage qimg(frame.data, frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
//        label1->setPixmap(QPixmap::fromImage(qimg));

//    }
//    else{
//        timer->stop();
//    }
}

void MainWindow::updateFrame2()
{
    cv::Mat frame2;
    //cap1 >> frame1;
    cap2 >> frame2;
    //cap3 >> frame3;

//    int width = label3->width();
//    cv::resize(frame3,frame3,cv::Size(width,width/16*9));

    cv::Mat frame2_rgb;
    //cv::cvtColor(frame1,frame1_rgb,cv::COLOR_BGR2RGB);
    cv::cvtColor(frame2,frame2_rgb,cv::COLOR_BGR2GRAY);

    //QImage img1(frame1_rgb.data, frame1_rgb.cols, frame1_rgb.rows, QImage::Format_RGB888);
    QImage img2(frame2_rgb.data, frame2_rgb.cols, frame2_rgb.rows, QImage::Format_Indexed8);
    //QImage img3(frame3.data, frame3.cols, frame3.rows, QImage::Format_RGB888);

    //label1->setPixmap(QPixmap::fromImage(img1));
    label2->setPixmap(QPixmap::fromImage(img2));
    //label3->setPixmap(QPixmap::fromImage(img3));

    //label1->setScaledContents(true);
    label2->setScaledContents(true);
    //label3->setScaledContents(true);

//    // Recording
//    if(isRecording){
//        writer.write(frame1);
//    }



//    cv::Mat frame;
//    if(cap3.read(frame)){
//        int width = label1->width();
//        cv::resize(frame,frame,cv::Size(width,width/16*9));
//        QImage qimg(frame.data, frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
//        label1->setPixmap(QPixmap::fromImage(qimg));

//    }
//    else{
//        timer->stop();
//    }
}

void MainWindow::updateFrame3()
{
    cv::Mat frame3;
    //cap1 >> frame1;
    //cap2 >> frame2;
    cap3 >> frame3;

//    int width = label3->width();
//    cv::resize(frame3,frame3,cv::Size(width,width/16*9));

    //cv::Mat frame1_rgb, frame2_rgb;
    //cv::cvtColor(frame1,frame1_rgb,cv::COLOR_BGR2RGB);
    //cv::cvtColor(frame2,frame2_rgb,cv::COLOR_BGR2GRAY);

    //QImage img1(frame1_rgb.data, frame1_rgb.cols, frame1_rgb.rows, QImage::Format_RGB888);
    //QImage img2(frame2_rgb.data, frame2_rgb.cols, frame2_rgb.rows, QImage::Format_Indexed8);
    QImage img3(frame3.data, frame3.cols, frame3.rows, QImage::Format_RGB888);

    //label1->setPixmap(QPixmap::fromImage(img1));
    //label2->setPixmap(QPixmap::fromImage(img2));
    label3->setPixmap(QPixmap::fromImage(img3));

    //label1->setScaledContents(true);
    //label2->setScaledContents(true);
    label3->setScaledContents(true);

//    // Recording
//    if(isRecording){
//        writer.write(frame1);
//    }



//    cv::Mat frame;
//    if(cap3.read(frame)){
//        int width = label1->width();
//        cv::resize(frame,frame,cv::Size(width,width/16*9));
//        QImage qimg(frame.data, frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
//        label1->setPixmap(QPixmap::fromImage(qimg));

//    }
//    else{
//        timer->stop();
//    }
}

void MainWindow::appendToLog(const QString &text)
{
    textBrowser->append(text);
}

void MainWindow::startRecording() {
    if (!isRecording) {
        isRecording = true;
        QString filename = "video_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmm") + ".avi"; // 현재 날짜와 시간을 포함한 동영상 파일명 지정
        int fourcc = VideoWriter::fourcc('X', '2', '6', '4'); // 코덱 설정
        int fps = 30; // 프레임 속도 설정
        Size frameSize(cap1.get(CAP_PROP_FRAME_WIDTH), cap1.get(CAP_PROP_FRAME_HEIGHT)); // 프레임 크기 설정
        writer.open(filename.toStdString(), fourcc, fps, frameSize);
        //appendToLog("영상 녹화 시작");
        QTimer::singleShot(60000,this, &MainWindow::stopAndStartRecording);
    }
}

void MainWindow::stopRecording() {
    if (isRecording) {
        isRecording = false;
        writer.release();
        //appendToLog("영상 녹화 종료");
    }
}

void MainWindow::stopAndStartRecording() {
    stopRecording();
    startRecording();
}
/*
void MainWindow::saveHourlyVideo() {

    if (isRecording && !isHourlyRecording) {
        isHourlyRecording = true;
        appendToLog("1시간 단위 녹화 시작");
        //qDebug() << "Hourly recording started.";
    } else if (isRecording && isHourlyRecording) {
        stopRecording();
        isHourlyRecording = false;
        appendToLog("1시간 단위 녹화 종료");
        //qDebug() << "Hourly recording stopped.";
    }

}
*/
void MainWindow::slot_M_input()
{
    appendToLog("메뉴 선택");
    MenuBtn->animateClick(100);
    if(MenuDialog->isHidden())
        MenuDialog->show();
    else {
        MenuDialog->hide();
    }
}

void MainWindow::slot_S_input()
{
    if (!isRecording) {
        startRecording();
        appendToLog("영상 녹화 시작");
    } else {
        stopRecording();
        appendToLog("영상 녹화 종료");
    }
}
void MainWindow::slot_Return_input()
{
    if(treewidget->currentItem()->text(0) == "K-16")
    {
        GunSelect->setText("K-16");
        appendToLog("K-16 선택");
    }
    else if(treewidget->currentItem()->text(0) == "M60")
    {
        GunSelect->setText("M60");
        appendToLog("M60 선택");
    }
    else
    {
        ;
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_M:
        emit key_M_Pushed();
        break;
    case Qt::Key_Up:
        emit key_Up_Pushed();
        break;
    case Qt::Key_Down:
        emit key_Down_Pushed();
        break;
    case Qt::Key_Left:
        emit key_Left_Pushed();
        break;
    case Qt::Key_Right:
        emit key_Right_Pushed();
        break;
    case Qt::Key_Return:
        emit key_Return_Pushed();
        break;
    case Qt::Key_S:
        emit key_S_Pushed();
        break;
    default:
        ;
        break;
    }
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QWidget *currentWidget;

    ui->stackedWidget->setCurrentIndex(1);
    currentWidget = ui->stackedWidget->currentWidget();
    dual_camera_widget = qobject_cast<dual_camera_display*>(currentWidget);


    ui->stackedWidget->setCurrentIndex(0);
    currentWidget = ui->stackedWidget->currentWidget();
    single_camera_widget = qobject_cast<single_camera_display*>(currentWidget);

    for(int i = 0; i < 2; i++){
        cap[i] = new cv::VideoCapture(i*2);
        dual_camera_widget->cap[i] = cap[i];
    }
    single_camera_widget->cap = cap[0];

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(66); // update Frame 30fps (1000(1s = 1000ms/30(fps) = 66(Frame rate)
    ui->single_camera_btn->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_single_camera_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->single_camera_btn->setEnabled(false);
    ui->dual_camera_btn->setEnabled(true);

}


void MainWindow::on_dual_camera_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->single_camera_btn->setEnabled(true);
    ui->dual_camera_btn->setEnabled(false);
}



void MainWindow::updateFrame(){
    int width,height;
    switch(ui->stackedWidget->currentIndex()){
    case 0:
        single_camera_widget->getCameraLabelSize(width,height);
        height = width/16*9; // 16:9 카메라 비율
        single_camera_widget->updateFrame(width,height);
        break;
    case 1:
        dual_camera_widget->getCameraLabelSize(width,height);
        height = width/16*9; // 16:9 카메라 비율
        dual_camera_widget->updateFrame(width,height);
        break;
    }
}


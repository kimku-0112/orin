#include "single_camera_display.h"
#include "ui_single_camera_display.h"
#include <QDebug>

single_camera_display::single_camera_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::single_camera_display)
{
    ui->setupUi(this);
}
single_camera_display::~single_camera_display()
{
    delete ui;
}

void single_camera_display::updateFrame(int width,int height){
    camera_open(cap,ui->camera_label,width,height);
}


void single_camera_display::getCameraLabelSize(int &width,int &height){
    width = ui->camera_label->width();
    height = ui->camera_label->height();
}

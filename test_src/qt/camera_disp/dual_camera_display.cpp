#include "dual_camera_display.h"
#include "ui_dual_camera_display.h"

dual_camera_display::dual_camera_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dual_camera_display)
{
    ui->setupUi(this);
}

dual_camera_display::~dual_camera_display()
{
delete ui;
}

void dual_camera_display::updateFrame(int width,int height){
    camera_open(cap[0],ui->camera1_label,width,height);
    camera_open(cap[1],ui->camera2_label,width,height);
}



void dual_camera_display::getCameraLabelSize(int &width,int &height){
    width = ui->camera1_label->width();
    height = ui->camera1_label->height();
}

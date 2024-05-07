#include "camera_fullsize.h"
#include "ui_camera_fullsize.h"

camera_fullsize::camera_fullsize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camera_fullsize)
{
    ui->setupUi(this);
}

camera_fullsize::~camera_fullsize()
{
    delete ui;
}

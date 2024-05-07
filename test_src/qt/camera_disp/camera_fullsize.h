#ifndef CAMERA_FULLSIZE_H
#define CAMERA_FULLSIZE_H

#include <QWidget>

namespace Ui {
class camera_fullsize;
}

class camera_fullsize : public QWidget
{
    Q_OBJECT

public:
    explicit camera_fullsize(QWidget *parent = nullptr);
    ~camera_fullsize();

private:
    Ui::camera_fullsize *ui;
};

#endif // CAMERA_FULLSIZE_H

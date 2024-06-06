#include "camera.h"
void CaptureOpenVideoFile(QWidget *parent, cv::VideoCapture *cap){
    QString filePath = QFileDialog::getOpenFileName(parent,
                                                    QObject::tr("Open Video File"),
                                                    QObject::tr("/home/rcws"),
                                                    QObject::tr("Video Files (*.mp4 *.avi *.mov);;"));
    if(!filePath.isEmpty()){
        cap->open(filePath.toStdString());
        if(!cap->isOpened()){
            QMessageBox::warning(parent,
                                 QObject::tr("Error"),
                                 QObject::tr("Could not open video file."));
        }
    }
    if(!cap->isOpened()){
        QMessageBox::warning(parent,
                             QObject::tr("Error"),
                             QObject::tr("Novideo file opened."));
    }
}

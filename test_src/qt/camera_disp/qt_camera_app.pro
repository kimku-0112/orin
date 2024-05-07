QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++0x

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp\
    camera_open.cpp \
    dual_camera_display.cpp \
    mainwindow.cpp \
    single_camera_display.cpp

HEADERS += \
    camera_open.h \
    dual_camera_display.h \
    mainwindow.h \
    single_camera_display.h

FORMS += \
    dual_camera_display.ui \
    mainwindow.ui \
    single_camera_display.ui

CONFIG += link_pkgconfig
PKGCONFIG += opencv4

INCLUDEPATH += /usr/include
DEPENDPATH += /usr/include


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

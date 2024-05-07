#ifndef COMM_THREAD_H
#define COMM_THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <fcntl.h>
#include <termios.h>
#include <thread>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <mutex>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QString>

#define SERIAL_PORT "/dev/ttyTHS1"

void UART_Init(struct termios &uart_config, int &uart);
void CANFD_Init(struct can_frame &frame, int &sock);

void write_uart(struct termios &uart_config, int &uart, char *buffer, int len);
void read_canfd(struct can_frame &frame, int &sock, unsigned int &id, int &len, char *data);
void write_candf(struct can_frame &frame, int &sock);

//==========================================================================================
class UartReadThread : public QThread{
    Q_OBJECT
public:
    explicit UartReadThread(struct termios *config,int *port, QMutex *mutex, QObject* parent = nullptr);
    ~UartReadThread() override;
private:
    struct termios *uart_config;
    int *uart; //uart port
    QMutex *mtx;
protected:
    void run() override;
};

//==========================================================================================
class UartWriteThread : public QThread{
    Q_OBJECT
public:
    explicit UartWriteThread(struct termios *config,int *port, QMutex *mutex, QObject* parent = nullptr);
    ~UartWriteThread() override;
private:
    struct termios *uart_config;
    int *uart;
    QMutex *mtx;
protected:
    void run() override;
};


//==========================================================================================
class CanReadThread : public QThread{
    Q_OBJECT
public:
    explicit CanReadThread(struct can_frame *frame, int *sock, QMutex *mutex, QObject* parent = nullptr);
    ~CanReadThread() override;
private:
    struct can_frame *canframe;
    int *cansock;
    QMutex *mtx;
protected:
    void run() override;
};


//==========================================================================================
class CanWriteThread : public QThread{
    Q_OBJECT
public:
    explicit CanWriteThread(struct can_frame *frame, int *sock, QMutex *mutex, QObject* parent = nullptr);
    ~CanWriteThread() override;
private:
    struct can_frame *canframe;
    int *cansock;
    QMutex *mtx;
protected:
    void run() override;
};

//==========================================================================================

#endif

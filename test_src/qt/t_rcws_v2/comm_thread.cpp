#include "comm_thread.h"
#include <QDebug>

void UART_Init(struct termios &uart_config, int &uart)
{
    uart = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart == -1)
    {
        perror("Unable to open UART port");
        exit(EXIT_FAILURE);
    }

    tcgetattr(uart, &uart_config);
    cfsetispeed(&uart_config, B115200); // Set baud rate
    cfsetospeed(&uart_config, B115200);
    uart_config.c_cflag &= ~PARENB; // No parity
    uart_config.c_cflag &= ~CSTOPB; // 1 stop bit
    uart_config.c_cflag &= ~CSIZE;
    uart_config.c_cflag |= CS8; // 8 data bits
    tcsetattr(uart, TCSANOW, &uart_config);
}

void CANFD_Init(struct can_frame &frame, int &sock)
{
    struct sockaddr_can addr;
    struct ifreq ifr;

    const char *ifname = "can0"; // 사용할 CAN 인터페이스 이름

    // 소켓 생성
    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock == -1)
    {
        perror("Error creating socket");
        return;
    }

    // CAN 인터페이스 이름 설정
    strcpy(ifr.ifr_name, ifname);
    ioctl(sock, SIOCGIFINDEX, &ifr);

    // 소켓 주소 설정
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // 소켓에 바인딩
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("Error binding socket");
        close(sock);
        return;
    }
}

void write_uart(struct termios &uart_config, int &uart, char *buffer, int len)
{
    if (write(uart, buffer, len) == -1)
    {
        perror("Error writing to UART");
    }
}

void read_canfd(struct can_frame &frame, int &sock, unsigned int &id, int &len, char *data)
{
    ssize_t nbytes = read(sock, &frame, sizeof(struct can_frame));
    if (nbytes < 0)
    {
        perror("Error reading frame");
        close(sock);
        return;
    }
    else if (nbytes < sizeof(struct can_frame))
    {
        std::cerr << "Incomplete CAN frame received\n";
        return;
    }
    id = frame.can_id;
    len = frame.can_dlc;
    std::memcpy(data, frame.data, len);
}
void write_candf(struct can_frame &frame, int &sock)
{
    int nbytes;
    // 메시지 전송
    nbytes = write(sock, &frame, sizeof(struct can_frame));
    if (nbytes == -1)
    {
        perror("Error writing frame");
        return;
    }
    else if (nbytes < sizeof(struct can_frame))
    {
        std::cerr << "Incomplete CAN frame written\n";
    }
    else
    {
        std::cout << "CAN frame sent successfully\n";
    }
}

//==========================================================================================
UartReadThread::UartReadThread(struct termios *config,int *port, QMutex *mutex, QObject *parent)
    :QThread(parent){
    uart_config = config;
    uart = port;
    mtx = mutex;
}

UartReadThread::~UartReadThread(){}

void UartReadThread::run(){
    char rx_buffer[64];
    while (1)
    {
        mtx->lock();
        int nbytes = read(*uart, rx_buffer, sizeof(rx_buffer) - 1);
        if (nbytes > 0)
        {
            printf("Received: %s\n", rx_buffer);
        }
        mtx->unlock();
    }
}
//==========================================================================================
UartWriteThread::UartWriteThread(struct termios *config,int *port, QMutex *mutex, QObject *parent)
    :QThread(parent){
    uart_config = config;
    uart = port;
    mtx = mutex;
}

UartWriteThread::~UartWriteThread(){}

void UartWriteThread::run(){
    int i = 0;
    char tx_buffer[64];
    while (1)
    {
        mtx->lock();
        int len = sprintf(tx_buffer, "tx test ok%d\r\n", i++);
        write_uart(*uart_config, *uart, tx_buffer, len);
        sleep(1);

        mtx->unlock();
        sleep(10);
    }
}

//==========================================================================================
CanReadThread::CanReadThread(struct can_frame *frame, int *sock, QMutex *mutex, QObject* parent)
    :QThread(parent){
    canframe = frame;
    cansock = sock;
    mtx = mutex;
}

CanReadThread::~CanReadThread(){}

void CanReadThread::run(){
    unsigned int id;
    int len;
    char rx_data[128];
    while (1)
    {
//        mtx->lock();
        read_canfd(*canframe, *cansock, id, len, rx_data);
        if (len > 0)
        {
            printf("CanFD Receive id: %x len: %d data:", id, len);
            for (int i = 0; i < len; i++)
            {
                printf("%02x ", rx_data[i]);
            }
            printf("\n");
        }
//        mtx->unlock();
    }
}

//==========================================================================================
CanWriteThread::CanWriteThread(struct can_frame *frame, int *sock, QMutex *mutex, QObject* parent)
    :QThread(parent){
    canframe = frame;
    cansock = sock;
    mtx = mutex;
}

CanWriteThread::~CanWriteThread(){}

void CanWriteThread::run(){
    int i = 0;
    while (1)
    {
//        mtx->lock();
        // 전송할 메시지 설정
        canframe->can_id = i++;   // 전송할 CAN ID 설정
        canframe->can_dlc = 8;    // 전송할 데이터 길이 설정
        canframe->data[0] = 0x11; // 전송할 데이터 설정
        canframe->data[1] = 0x22;
        canframe->data[2] = 0x33;
        canframe->data[3] = 0x44;
        canframe->data[4] = 0x55;
        canframe->data[5] = 0x66;
        canframe->data[6] = 0x77;
        canframe->data[7] = 0x88;
        write_candf(*canframe, *cansock);
//        mtx->unlock();
        sleep(1);
    }
}

//==========================================================================================



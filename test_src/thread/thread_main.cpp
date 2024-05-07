// compile command
// ==================================
// g++ thread_main.cpp -o thread_main
// ==================================

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

#define SERIAL_PORT "/dev/ttyTHS1"

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
void read_uart(struct termios &uart_config, int &uart, int &nbytes, char *data)
{
    nbytes = read(uart, data, sizeof(data) - 1);
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

    // CAN FD 프레임인 경우만 처리
    // if (frame.can_id & CAN_EFF_FLAG)
    // {
    //     std::cout << "Can Extended Frame ID: 0x" << std::hex << frame.can_id << std::dec << "\n";
    // }
    // else
    // {
    //     std::cout << "Can Standard Frame ID: 0x" << std::hex << frame.can_id << std::dec << "\n";
    // }

    // std::cout << "Data Length: " << frame.can_dlc << "\n";
    // std::cout << "Data: ";
    // for (int i = 0; i < frame.can_dlc; ++i)
    // {
    //     std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)frame.data[i] << " ";
    // }
    // std::cout << std::dec << "\n\n";
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

void process_read_uart_thread(std::mutex &mtx, struct termios &uart_config, int &uart)
{
    char rx_buffer[64];
    while (1)
    {
        int nbytes = read(uart, rx_buffer, sizeof(rx_buffer) - 1);
        if (nbytes > 0)
        {
            printf("Received: %s\n", rx_buffer);
        }
        sleep(1);
    }
}
void process_write_uart_thread(std::mutex &mtx, struct termios &uart_config, int &uart)
{
    int i = 0;
    char tx_buffer[64];
    while (1)
    {
        mtx.lock();
        int len = sprintf(tx_buffer, "tx test ok%d\r\n", i++);
        write_uart(uart_config, uart, tx_buffer, len);
        mtx.unlock();
        sleep(1);
    }
}

void process_read_canfd_thread(std::mutex &mtx, struct can_frame &frame, int &sock)
{
    while (1)
    {
        unsigned int id;
        int len;
        char rx_data[128];
        read_canfd(frame, sock, id, len, rx_data);
        if (len > 0)
        {
            printf("CanFD Receive id: %x len: %d data:", id, len);
            for (int i = 0; i < len; i++)
            {
                printf("%02x ", rx_data[i]);
            }
            printf("\n");
        }
        sleep(1);

        // char *output;
        // int output_size = 0;
        // // id와 len을 문자열로 추가
        // output_size += snprintf(NULL, 0, "CanFD Receive id: %x len: %d data:", id, len);
        // output = (char *)malloc(output_size + 1);
        // snprintf(output, output_size + 1, "CanFD Receive id: %x len: %d data:", id, len);

        // // 데이터를 16진수 형식으로 문자열에 추가
        // for (int i = 0; i < len; ++i)
        // {
        //     // 문자열 길이를 재계산하여 데이터를 추가
        //     int data_size = snprintf(NULL, 0, " %x", (unsigned char)data[i]);
        //     output = (char *)realloc(output, output_size + data_size + 1);
        //     snprintf(output + output_size, data_size + 1, " %x", (unsigned char)data[i]);
        //     output_size += data_size;
        // }
    }
}

void process_write_canfd_thread(std::mutex &mtx, struct can_frame &frame, int &sock)
{
    int i = 0;
    while (1)
    {
        // 전송할 메시지 설정
        mtx.lock();
        frame.can_id = i++;   // 전송할 CAN ID 설정
        frame.can_dlc = 8;    // 전송할 데이터 길이 설정
        frame.data[0] = 0x11; // 전송할 데이터 설정
        frame.data[1] = 0x22;
        frame.data[2] = 0x33;
        frame.data[3] = 0x44;
        frame.data[4] = 0x55;
        frame.data[5] = 0x66;
        frame.data[6] = 0x77;
        frame.data[7] = 0x88;
        write_candf(frame, sock);
        mtx.unlock();
        sleep(1);
    }
}
int main()
{
    std::mutex mtx;

    struct termios uart_config;
    int uart;
    UART_Init(uart_config, uart);

    int sock;
    struct can_frame frame;
    CANFD_Init(frame, sock);

    std::thread read_uart_thread(process_read_uart_thread, std::ref(mtx), std::ref(uart_config), std::ref(uart));
    std::thread write_uart_thread(process_write_uart_thread, std::ref(mtx), std::ref(uart_config), std::ref(uart));
    std::thread read_canfd_thread(process_read_canfd_thread, std::ref(mtx), std::ref(frame), std::ref(sock));
    std::thread write_canfd_thread(process_write_canfd_thread, std::ref(mtx), std::ref(frame), std::ref(sock));

    read_uart_thread.join();
    write_uart_thread.join();
    read_canfd_thread.join();
    write_canfd_thread.join();

    return 0;
}

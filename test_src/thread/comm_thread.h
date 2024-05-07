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

void UART_Init(struct termios &uart_config, int &uart);
void CANFD_Init(struct can_frame &frame, int &sock);

void write_uart(struct termios &uart_config, int &uart, char *buffer, int len);
void read_canfd(struct can_frame &frame, int &sock, unsigned int &id, int &len, char *data);
void write_candf(struct can_frame &frame, int &sock);
void process_read_uart_thread(std::mutex &mtx, struct termios &uart_config, int &uart);
void process_write_uart_thread(std::mutex &mtx, struct termios &uart_config, int &uart);
void process_read_canfd_thread(std::mutex &mtx, struct can_frame &frame, int &sock);
void process_write_canfd_thread(std::mutex &mtx, struct can_frame &frame, int &sock);
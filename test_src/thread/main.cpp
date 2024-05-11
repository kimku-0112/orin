#include "comm_thread.h"
#define SERIAL_PORT "/dev/ttyUSB0"
// #define SERIAL_PORT /deb/ttyTHS0

int main()
{
    std::mutex mtx;

    struct termios uart_config;
    int uart;
    UART_Init(uart_config, uart, SERIAL_PORT);

    int sock;
    struct can_frame frame;
    CANFD_Init(frame, sock);

    std::thread read_uart_thread(process_read_uart_thread, std::ref(mtx), std::ref(uart_config), std::ref(uart));
    std::thread write_uart_thread(process_write_uart_thread, std::ref(mtx), std::ref(uart_config), std::ref(uart));
    std::thread read_canfd_thread(process_read_canfd_thread, std::ref(mtx), std::ref(frame), std::ref(sock));
    std::thread write_canfd_thread(process_write_canfd_thread, std::ref(mtx), std::ref(frame), std::ref(sock));

    read_uart_thread.join();
    // write_uart_thread.join();
    read_canfd_thread.join();
    // write_canfd_thread.join();

    return 0;
}

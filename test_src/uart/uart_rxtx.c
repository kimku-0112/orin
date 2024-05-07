
// compile : gcc uart_rxtx.c -o uart_test
// execute : ./uart_test
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define SERIAL_PORT "/dev/ttyTHS0" // UART device file

int main() {
    int uart_fd,i;
    int rx_length;
    char tx_buffer[64];
    char rx_buffer[256];
    struct termios uart_config;

    // Open UART port
    uart_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_fd == -1) {
        perror("Unable to open UART port");
        exit(EXIT_FAILURE);
    }

    // Configure UART
    tcgetattr(uart_fd, &uart_config);
    cfsetispeed(&uart_config, B115200); // Set baud rate
    cfsetospeed(&uart_config, B115200);
    uart_config.c_cflag &= ~PARENB; // No parity
    uart_config.c_cflag &= ~CSTOPB; // 1 stop bit
    uart_config.c_cflag &= ~CSIZE;
    uart_config.c_cflag |= CS8; // 8 data bits
    tcsetattr(uart_fd, TCSANOW, &uart_config);

    // Transmit data
    //char tx_buffer[] = "Hello UART!";
    for(i=0;i<0x10;i++)
    {
	    sprintf(tx_buffer,"tx test ok %d\r\n",i);
	    if (write(uart_fd, tx_buffer, strlen(tx_buffer)) == -1) 
	    {
		perror("Error writing to UART");
	    }
    }

    // Receive data

    for(i=0;i<100000;i++)
    {
	    rx_length = read(uart_fd, rx_buffer, sizeof(rx_buffer) - 1);
	    if (rx_length > 0) {
		rx_buffer[rx_length] = '\0';
		printf("Received: %s\n", rx_buffer);
	    } else if (rx_length == -1) {
		perror("Error reading from UART");
	    }
	    sleep(1);
    }

    close(uart_fd);
    return 0;
}


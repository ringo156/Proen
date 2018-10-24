#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "uart_mcu.hpp"

#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE B9600


// void tx_buf(const char[] string){
//
// }

int main(){
    int fd;
    struct termios orgtio;

    serial_init(BAUD_RATE, &fd, SERIAL_PORT, &orgtio);

    char buf[] = "test\n";
    write(fd, buf, sizeof(buf));

    ioctl(fd, TCSETS, &orgtio);
    close(fd);

    return 0;
}

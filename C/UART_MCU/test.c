#include "uart_mcu.h"
#include <stdio.h>

#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE B19200


// void tx_buf(const char[] string){
//
// }

int main(){
    int fd;
    char buf[50];

    serial_init(BAUD_RATE, &fd);

    while(1){
        sprinf(buf, "test\r\n");
        write(fd, buf, sizeof(buf));
    }

    close(fd);
    return 0;
}

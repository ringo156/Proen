#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>

//#include "serial_servo.hpp"
//#include "wait_msec_nsec_180301.h"
#include "uart_mcu.h"

#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE B9600
void Pos_Set(uint8_t ID, int16_t kakudo);

int main(){
    int fd;
    struct termios orgtio;

    serial_init(BAUD_RATE, &fd, SERIAL_PORT, &orgtio);

    int16_t kakudo = 0;
    int16_t Data;
    uint8_t i, tx[3], rx[6];
    uint8_t ID = 0x01;
    Data = 7500 -( 30 * kakudo );
    tx[0]=0x80|ID;
    tx[1]=(unsigned char)(Data>>7) & 0x7F;
    tx[2]=(unsigned char)Data & 0x7F;
    write(fd, tx, sizeof(tx));

    // char buf[] = "testData\n";
    // write(fd, buf, sizeof(buf));
    sleep(1);
    ioctl(fd, TCSETS, &orgtio);
    close(fd);

    return 0;
}


void Pos_Set(uint8_t ID, int16_t kakudo){
    //int pos_h,pos_l,pos;
    int16_t Data;
    uint8_t i, tx[3], rx[6];
    Data = 7500 -( 30 * kakudo );
    tx[0]=0x80|ID;
    tx[1]=(unsigned char)(Data>>7) & 0x7F;
    tx[2]=(unsigned char)Data & 0x7F;
    
    // UART_servo_PutArray(tx,3);
    // CyDelay(1);

}

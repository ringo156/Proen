#include <iostream>
#include "serial_servo.hpp"

#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE B9600

// serial_init(int baudRate, int *fd, const char serial_port[], struct termios *orgtio);

int main(){

    serialServo servo(BAUD_RATE, SERIAL_PORT);

    //serial_init(BAUD_RATE, &fd, SERIAL_PORT, &orgtio);

    char buf[] = "test\n";
    // write(fd, buf, sizeof(buf));

    servo.writeArray(buf, sizeof(buf));

    // ioctl(fd, TCSETS, &orgtio);
    // close(fd);

    return 0;
}

// int serial_init(int baudRate, int *fd, const char serial_port[], struct termios *orgtio){
//     unsigned char msg[] = "serial port open...\n";
//     struct termios tio;
//     *fd = open(serial_port, O_RDWR);     // デバイスをオープンする
//     if (*fd < 0) {
//         printf("open error\n");
//         return -1;
//     }

//     ioctl(*fd, TCGETS, orgtio);
//     tio = *orgtio;

//     tio.c_cflag += CREAD;               // 受信有効
//     tio.c_cflag += CLOCAL;              // ローカルライン（モデム制御なし）
//     tio.c_cflag += CS8;                 // データビット:8bit
//     tio.c_cflag += 0;                   // ストップビット:1bit
//     tio.c_cflag += 0;                   // パリティ:None

//     cfsetispeed(&tio, baudRate );
//     cfmakeraw(&tio);                    // RAWモード

//     tcsetattr( *fd, TCSANOW, &tio );     // デバイスに設定を行う

//     ioctl(*fd, TCSETS, &tio);            // ポートの設定を有効にする

//     printf("connect open\n");

//     return 0;

// }
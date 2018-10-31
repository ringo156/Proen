/* serial_servo.hpp */
// シリアルサーボのクラスとシリアルポートの初期化を分けたほうが良さそう

#ifndef _SERIAL_SERVO_H_
#define _SERIAL_SERVO_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "uart_mcu.h"

using namespace std;


class serialServo{
    private:
        int fd; //file descriptor
        struct termios orgtio;

    public:
    serialServo(int baudRate, const char serialPort[]){
        serial_init(baudRate, &fd, serialPort, &orgtio);

    }
    ~serialServo(){
        cout << "connect close" << endl;
        ioctl(fd, TCSETS, &orgtio);
        close(fd);
    }
    void writeArray(const char buf[], int size){
        write(fd, buf, size);
    }
};


#endif // _SERIAL_SERVO_H_
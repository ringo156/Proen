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


class serialServo{
    private:
        int baudRate;
        int *fd; //file descriptor
        const char serial_port[];
        struct termios *orgtio;


    public:
    serialServo(){

    }
    ~serialServo(){

    }
};


#endif // _SERIAL_SERVO_H_
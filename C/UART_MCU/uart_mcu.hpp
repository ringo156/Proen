//$ sudo chmod 666 /dev/ttyUSB0

#ifndef UART_MCU_H
#define UART_MCU_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int serial_init(int baudRate, int *fd, const char serial_port[]);
void read_buf(int *);

#endif

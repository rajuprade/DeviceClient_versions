
#ifndef USB_RS485_H_INCLUDED
#define USB_RS485_H_INCLUDED

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include<string.h>
#include <sys/io.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
#include<time.h>
#include<fcntl.h>
#include"/home/raj/Desktop/FPS_MCM/ftd2xx.h" 

int resp_code;
char *buf_ptr0;
char buff[];

typedef struct
 {
    int Length;
    char Msg[200];
 }  serialBuff ;

serialBuff wbuff,rbuff;

void add_cksum(char buff[]);
void write_read(char *buff, char *buf_ptr0,serialBuff rbuff);
//#include"ftd2xx.h"

//FILE *f,*fp;
#define BUF_SIZE  0x10
unsigned char Arg[100];

//#define DEVICE_ID "FT07T5Q2"
#define DEVICE_ID "FTSERRS1"

#define RABBIT_MCM5 1

#endif

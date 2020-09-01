#ifndef RESPONSE_H_INCLUDED
#define RESPONSE_H_INCLUDED
#include<math.h>
#define IDLE_MODE 1
#define SCAN_MODE 2
#define MEAN_MODE 3
#define LIMITS_MODE 4

int noise_cal;
char tbuf[64],abuf[64];
int resp_msg_cntr;
int resp_code;
int Monparam[12];
double pow_db;
unsigned long sec_tm;
float Monvoltbis[22];
 unsigned char tmpbuf[200], stmpbuf[256];
void copyTmpBuf(char *, char *, int);
 void copyTmpBuffe(char *, char *, int);
 void copyTmpBufcb(char *, char *, int);
 double convertdB (float , int );
 void decodeFEBox(int);
 void decodeCommonBox( );
//struct tm  rtc;
unsigned long t0, sec_tm;

#endif

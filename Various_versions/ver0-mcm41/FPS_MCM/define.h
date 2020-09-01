#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>


#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <netdb.h>
#include <setjmp.h>
#include <signal.h>
#include<time.h>

#define CMS_IP "192.168.3.35"

#define WRPR_PORT  3004    
#define WRPR_IP  "192.168.8.192"
   
#define RESP_PORT  5000
#define RESP_IP  "192.168.3.35"

#define EVNT_PORT 5001
#define EVNT_IP  "192.168.3.35"
  
#define MON_PORT  19999
#define MON_IP   "192.168.3.35"
      
#define SUBSYS_ID "SimulatedSys"   // SimulatedSys if not defined
#define DEVICE_IP "192.168.3.6" 

typedef enum { reqPORT, resPORT, evntPORT, monPORT } serverPorts;


#define SERVER_ADDR    "192.168.8.192"  // Server for LAB testing
#define SERVER_PORT    "3004"           // For 192.168.8.143


#define TCP_BUF_SIZE    16392
#define MAX_SOCKETS     4
#define MAX_TCP_SOCKET_BUFFERS 4
#define SCAN_DELAY		1000		// Doing background monitoring at every 4000 miliseconds

void Set_DMask();
void serial_init();
char chksum(char *ary);



float temp;

#define ERROR      (-1)


#define CINBUFSIZE 31
#define COUTBUFSIZE 31
#define RS232_NOCHARASSYINBRK
#define MSSG_TMOUT 5UL
#define IDLE_TMOUT 50UL

typedef enum { CURR_ENC, TARGET_ENC, CURR_ANG, TARGET_ANG, CURR_RPM, MODE, FPS_VER, RAMP_DNTIME,
RAMP_UPTIME, LOW_RPM_LMT, LOW_RPM_TIME, BRK_CNT_DIFF, STEP_SLW_TIMECNT,
FINE_TUNE_PWM, MAX_PWM, MAX_ANG, MIN_ANG, UA0_ANG, OVER_CURR, COMMERR,
CMDFAIL, TIMEOUT_CNT, CHKSUM_CNT, RST_CNT, FPS_CTRL_INFO, FPS_MOTOR_TEMP,
MOTOR_CURR, DOMON_STAT, TNG_PT, STOP_CNT, TVAR } ENUM_FPSPARA;

char par_value[TVAR][64];
char in_d[32];
//int bytecntr_in=0;
int FPS_resp_msg_cntr ;
int FPS_resp_code ;
char out_data[32], mcm_addr;

char temps[10], state[10];

char msw[5], lsw[5];

float temp, i1, i2, i3, i4, p1, p2, p3, p4;

char lt1i1[10], lt2i2[10], lt3i3[10], lt4i4[10], lt1p1[10], lt2p2[10], lt3p3[10], lt4p4[10];

char Attn1_Mon[10], Attn2_Mon[10], temp1[10];

int Ch1Att, Ch2Att;

//**************************** Defination part for Rabbit Card *************************//
#define ADC_SCLKBRATE 	115200ul
#define GAINSET 		   GAIN_1
#define CHANNEL_1 		0
#define CHANNEL_2 		1
#define CHANNEL_3 		2
#define CHANNEL_4 		3

#define MAXDATA       	128
#define DATALEN       	64
#define MSGLEN		128

#define USE_RABBITWEB 1
#define USE_HTTP_DIGEST_AUTHENTICATION 1

typedef enum { WRAPPER=10, INTERMEDIATE=11, FINAL=12, ASYNCH=13, ALARM=100 } EventEnum ;
typedef enum { SUCCESS=10, FAILED=11, SYNTX=12, INCOMPLETE=13, INVALID=14, UNKNOWNERR=100 } CodeEnum ;

int reset1;

#ifndef MY_IP_ADDR
char MY_IP_ADDR[20], MY_GATEWAY[20], MY_NET_MASK[20];
#endif

char ip[20], mask[20], gateway[20], psw[10];

int SS, FDB, FDV, Temp_SS, Temp_FDB, Temp_FDV;

char SS1_Mon[10], SS2_Mon[10], FDB_Mon[5], FDV_Mon[5], MCM_Clk[10];

struct tm  rtc;
unsigned long t0, sec_tm;

char time1[10], date[12];

int rw[70];

char vl[70][10], mswT[5], lswT[5];

char temp_ar[10], time_ar[10] ;

char RTC_time[20];

#pragma pack(push,1)
typedef struct
 {
    short  id;
    short  level;
    char description[MSGLEN];
 } RespAlarm;

typedef struct
 {
    short numpkt;
    char prmname[MAXDATA][DATALEN];
    char prmvalue[MAXDATA][DATALEN];
 } DataPkt;

// Basic Fields Information resued by XMLIB to form XML ACK/NACK/RESPONSE //
typedef struct
 {
    long  seq;                  // Sequence number of Command
    float version;
    short priority ;
    short timeout ;
    char  subsysid[DATALEN];
    char  cmd_name[DATALEN];        // Command name at user level
    char  timestamp[DATALEN];
    char  cmd_id[8];           // Command ID Device Specific
 } BasicFlds;

typedef struct
 {
    short code;
    short event ;
    char message[MSGLEN];

    RespAlarm MCM_alarm;
    DataPkt resp_data;
    BasicFlds resp_elem;
 } devResponse;

typedef struct
 {
    BasicFlds cmdelem;
    DataPkt cmd_data;
 } parseCMSCmd;


#pragma pack(pop)
#define parseCMSCmdSize  sizeof(parseCMSCmd)
#define devResponseSize  sizeof(devResponse)

parseCMSCmd ser_Cmd;
devResponse dev_Resp;


char dmask[10];

#endif

//********************************************************************************************//

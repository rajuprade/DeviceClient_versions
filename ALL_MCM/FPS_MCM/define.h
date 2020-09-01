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
#define DOMON_TIMEOUT  60000000  // 1.5m

#define CMS_IP "192.168.3.35"

#define WRPR_PORT  3004    
#define WRPR_IP  "192.168.70.21"

#define RESP_PORT  5000
#define RESP_IP  "192.168.3.35"

#define EVNT_PORT 5001
#define EVNT_IP  "192.168.3.35"
  
#define MON_PORT  19999
#define MON_IP   "192.168.3.35"
      
#define SUBSYS_ID "SimulatedSys"   // SimulatedSys if not defined
#define DEVICE_IP "192.168.3.6" 

typedef enum { reqPORT, resPORT, evntPORT, monPORT } serverPorts;


#define SERVER_ADDR    "192.168.34.2"  // Server for LAB testing
#define SERVER_PORT    "3004"           // For 192.168.8.143


#define FPS_FIFO "/tmp/fpscmd_fifo"

#define RESP_FIFO "/tmp/resp_fps"

char *fpsfifo;
int fps_fd,resp_fpsfd;    /* FIF0 File Descriptor for writing FECB command, Which can be read by USb-RS485 program */ 

char *respfps;

 
#define RESP_FIFO "/tmp/resp_fps"

typedef struct
 {
    int Length;
    char Msg[200];
 }  serialBuff ;

serialBuff wbuff,rbuff;

char *buf_ptr0;

int resp_code;

void response_decode(void);


float temp;

#define ERROR      (-1)




typedef enum { CURR_ENC, TARGET_ENC, CURR_ANG, TARGET_ANG, CURR_RPM, MODE, FPS_VER, RAMP_DNTIME,
RAMP_UPTIME, LOW_RPM_LMT, LOW_RPM_TIME, BRK_CNT_DIFF, STEP_SLW_TIMECNT,
FINE_TUNE_PWM, MAX_PWM, MAX_ANG, MIN_ANG, UA0_ANG, OVER_CURR, COMMERR,
CMDFAIL, TIMEOUT_CNT, CHKSUM_CNT, RST_CNT, FPS_CTRL_INFO, FPS_MOTOR_TEMP,
MOTOR_CURR, DOMON_STAT, TNG_PT, STOP_CNT, TVAR } ENUM_FPSPARA;

char par_value[TVAR][64];
unsigned char in_d[32];
int FPS_resp_msg_cntr ;
int FPS_resp_code ;
char out_data[32], mcm_addr;


#define MAXDATA       	128
#define DATALEN       	64
#define MSGLEN		128



typedef enum { WRAPPER=10, INTERMEDIATE=11, FINAL=12, ASYNCH=13, ALARM=100 } EventEnum ;
typedef enum { SUCCESS=10, FAILED=11, SYNTX=12, INCOMPLETE=13, INVALID=14, UNKNOWNERR=100 } CodeEnum ;



char temp_ar[20], time_ar[20] ;

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
    int  seq;                  // Sequence number of Command
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


#endif

//********************************************************************************************//:

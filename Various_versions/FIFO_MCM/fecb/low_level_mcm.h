#ifndef LOW_LEVEL_MCM_H_INCLUDED
#define LOW_LEVEL_MCM_H_INCLUDED

#include"define.h"

char *buf_ptr0;

typedef struct
 {
    int CmdNum ;
    int McmPow ;
    int feBoxNo[2];
    unsigned char Argument[100];
 } execcmd;
execcmd exeCmd;

typedef struct
 {
    int Length;
    char Msg[200];
 }  serialBuff ;

serialBuff wbuff,rbuff;

void add_cksum(unsigned char buff[]);


void set_dig_mask64();
void set_dig_mask32();
void set_dig_mask16();
void set_anl_mask();
void set_mean_mode();
void feed_cntrl_old();
void feed_cntrl_new();
void fe_box_mon();
void FECB_CTRL(int MCMADDR );

#endif

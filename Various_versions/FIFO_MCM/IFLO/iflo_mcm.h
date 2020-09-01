#ifndef IFLO_MCM_H_INCLUDED
#define IFLO_MCM_H_INCLUDED

int resp_msg_cntr;
char mcm_addr;
double pow_db;
//char mask1,mask2,mask3,mask4;
char mask1[6], mask2[6], mask3[6], mask4[6];
//extern void Mask32toArg( mask1,  mask2);
//extern void Mask64toArg( mask1,  mask2,  mask3,mask4);
void Convert(int offset, char * mask_x);
//void Mask32toArg(char mask0, char mask1);
//void Mask64toArg(char mask0, char mask1, char mask2, char mask3);
void Mask32toArg(char * mask1, char * mask2);
void Mask64toArg(char * mask1, char * mask2, char * mask3, char * mask4);
int ifloCtrlCmd(parseCMSCmd *Wrpr_Cmd);
void IFLO_CTRL(int MCMADDR );

int set_lo();
void set_if();

 typedef struct
 {
    int cmdCode;
    int lo[2];
    int if_band[2]; 
    float if_pa[2];
    int if_pg[2];
    int if_alc[2];
    
 } ifloset;

 typedef struct
 {
    int CmdNum ;
    int McmPow ;
    int feBoxNo[2];
    unsigned char Argument[256];
 } execcmd;

 typedef struct
 {
    int Length;
    char Msg[200];
 } serialBuff ;


 ifloset ifloSet;
 execcmd exeCmd;
 serialBuff wbuff;
 serialBuff rbuff;
 char *buf_ptr0;
 unsigned short bitD0[8], mcm4word[2];
 unsigned short digWord[32];



#endif

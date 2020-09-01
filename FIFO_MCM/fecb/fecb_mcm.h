#ifndef FECB_MCM_H_INCLUDED
#define FECB_MCM_H_INCLUDED


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
int fecbCtrlCmd(parseCMSCmd *Wrpr_Cmd);
extern void FECB_CTRL(int MCMADDR );
 typedef struct
 {
    int cmdCode;
    int freq_set[2];
    int solar_attn[2]; 
    int polSwap, noiseCal, ngCycle,ngFreq;
    int walshGrp, walshSW,walshPat,walshFreq;
    int rfcmSW, ngSW;
 } fecbset;

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


 fecbset fecbSet;
 execcmd exeCmd;
 serialBuff wbuff;
 serialBuff rbuff;
 char *buf_ptr0;
 unsigned short bitD0[8], mcm4word[2];
 unsigned short digWord[32];
void mcm4settingfordomon();


#endif

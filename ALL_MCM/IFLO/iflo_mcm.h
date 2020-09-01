#ifndef IFLO_MCM_H_INCLUDED
#define IFLO_MCM_H_INCLUDED


//char mcm_addr;
//double pow_db;
//char mask1,mask2,mask3,mask4;
char mask1[6], mask2[6], mask3[6], mask4[6];

void Convert(int offset, char * mask_x);

void Mask16toArg(char * mask1);
void Mask32toArg(char * mask1, char * mask2);
void Mask64toArg(char * mask1, char * mask2, char * mask3, char * mask4);
int ifloCtrlCmd(parseCMSCmd *Wrpr_Cmd);
void IFLO_CTRL(int MCMADDR );

int ofc_ch1,ofc_ch2;

int set_lo();
void set_if();
void set_ofcattn(); // OFC attenuation Function

void servo_reset();
void fps_reset(); 
void mcm4setting();

char maskofc[12];   // Used for generating mask bit for OFC Attenuation,Servo RESET and FPS reset 
int mask_16bit,change_mask; 



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


ifloset ifloSet;
fecbset fecbSet;

 execcmd exeCmd;
 serialBuff wbuff;
 serialBuff rbuff;
 char *buf_ptr0;
 unsigned short bitD0[8], mcm4word[2];
 unsigned short digWord[32];



#endif

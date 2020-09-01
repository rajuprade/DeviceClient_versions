//**************************** Defination part for FE-CB System *************************//

#ifndef FECB_ETH_H_INCLUDED
#define FECB_ETH_H_INCLUDED

//#include "define.h"
//#include "fecb_mcm.h"
#define SYSTEM_2	    	"fecb"
#define Cmd_200			"init"
#define Cmd_201			"null"
#define Cmd_202			"setrfsys"
#define Cmd_203			"domon"
#define Cmd_204			"sel_febox"
#define Cmd_205			"sel_ufebox"
#define Cmd_206			"rfswap"
#define Cmd_207			"rfattn"
#define Cmd_208			"rfon"
#define Cmd_209			"cbterm"
#define Cmd_210			"rfterm"
#define Cmd_211			"rfngset"
#define Cmd_212			"rfngcal"
#define Cmd_213			"setwalsh"
#define Cmd_214			"sel_walshgrp"
#define Cmd_215			"seturfsys"
#define Cmd_216			"rfcm_sw"
#define Cmd_217			"rawmon"
#define Cmd_218			"restore"
#define Cmd_219			"settime"
#define Cmd_220			"rebootmcm"
//#define Cmd_206			"sel_fefilter_ch1"
//#define Cmd_207			"sel_fefilter_ch2"
//#define Cmd_208			"rfband"



int form_inter_resp(parseCMSCmd *, devResponse *);
int cmd_validation(parseCMSCmd *, devResponse *);
void fillSetpara(parseCMSCmd *, devResponse *);
void FE_form_final_resp(parseCMSCmd *, devResponse *);
int command_process_FE(parseCMSCmd *, devResponse *);

/* void  formDeviceResp(parseCMSCmd *, devResponse *);
void sendFinResp(devResponse *, int );
void sendIntmResp(devResponse *, int ); */

#endif

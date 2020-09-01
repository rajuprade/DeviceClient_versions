//**************************** Defination part for FE-CB System *************************//

#ifndef IFLO_ETH_H_INCLUDED
#define IFLO_ETH_H_INCLUDED

#define SYSTEM_5	    	"sigcon"
#define Cmd_500			"setilo"
#define Cmd_501			"setif"
#define Cmd_502			"setofatten"
#define Cmd_503			"domon"
#define Cmd_504			"srvrst"
#define Cmd_505			"fpsrst"
#define Cmd_506			"setsigcon" 
#define Cmd_507			"setifatten"
#define Cmd_508			"walshpatern"   // SET WALSH pattern ( 0,7) 
#define Cmd_509			"setwalshgrp"    // set walsh group 
#define Cmd_510			"rfcm_sw"       // MCM 5 on /OFF 
#define Cmd_511			"walshfreq"     // WALSH FREQUENCY (0,3) 
#define Cmd_512			"noisefreq"     // NOISE FREQUENCY (1,3) 
#define Cmd_513			"setrfnoise"    // NG Duty Cycle 
#define Cmd_514			"setwalsh"     // Set walsh 

int form_inter_resp(parseCMSCmd *, devResponse *);
int cmd_validation(parseCMSCmd *, devResponse *);
void fillSetpara(parseCMSCmd *, devResponse *);
void IFLO_form_final_resp(parseCMSCmd *, devResponse *);
int command_process_IFLO(parseCMSCmd *, devResponse *);

#endif

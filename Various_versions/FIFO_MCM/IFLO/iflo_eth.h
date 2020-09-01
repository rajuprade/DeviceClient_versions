//**************************** Defination part for FE-CB System *************************//

#ifndef IFLO_ETH_H_INCLUDED
#define IFLO_ETH_H_INCLUDED

#define SYSTEM_5	    	"iflo"
#define Cmd_500			"set_lo"
#define Cmd_501			"set_if"
#define Cmd_502			"null"
#define Cmd_503			"domon"



int form_inter_resp(parseCMSCmd *, devResponse *);
int cmd_validation(parseCMSCmd *, devResponse *);
void fillSetpara(parseCMSCmd *, devResponse *);
void IFLO_form_final_resp(parseCMSCmd *, devResponse *);
int command_process_IFLO(parseCMSCmd *, devResponse *);

#endif

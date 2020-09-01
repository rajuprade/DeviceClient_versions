//**************************** Defination part for FPS System *************************//
#ifndef FPS_ETH_H_INCLUDED
#define FPS_ETH_H_INCLUDED

#define SYSTEM_4		"fps"
#define Cmd_401			"domon"
#define Cmd_402			"set_tng_pnt"
#define Cmd_403			"set_rmp_dncnt"
#define Cmd_404			"set_lrpm_lmt"
#define Cmd_405			"set_brkcnt_diff"
#define Cmd_406			"set_rpm_upcnt"
#define Cmd_407			"set_stop_cnt"
#define Cmd_408			"set_max_ang"
#define Cmd_409			"set_min_ang"
#define Cmd_410			"set_max_pwm"
#define Cmd_411			"rd_tng_pnt"
#define Cmd_412			"rd_rmp_dncnt"
#define Cmd_413			"rd_lrpm_lmt"
#define Cmd_414			"rd_brkcnt_diff"
#define Cmd_415			"rd_rmp_upcnt"
#define Cmd_416			"rd_stop_cnt"
#define Cmd_417			"rd_max_ang"
#define Cmd_418			"rd_min_ang"
#define Cmd_419			"rd_max_pwm"
#define Cmd_420			"rd_version"
#define Cmd_421			"rd_ua0_ang"
#define Cmd_422			"calibrate"
#define Cmd_423			"runfree"
#define Cmd_424			"ua0calib"
#define Cmd_425			"runcpreset"
#define Cmd_426			"rundpreset"
#define Cmd_427			"finectune"
#define Cmd_428			"finedtune"
#define Cmd_429			"fpsboot"
#define Cmd_430			"fpsstop"
#define Cmd_431			"ldfpspos"
#define Cmd_432			"mvfps150"
#define Cmd_433			"mvfps375"
#define Cmd_434			"mvfps700"
#define Cmd_435			"mvfps1420"
#define Cmd_436			"initfps"      // Batch command can be implemented in later stage.
#define Cmd_437			"reset"
#define Cmd_438			"settime"


int FPS_cmd_validation(parseCMSCmd *, devResponse *);
int FPS_form_inter_resp(parseCMSCmd *, devResponse *);
void FPS_form_final_resp(parseCMSCmd *, devResponse *);
int command_process_fps(parseCMSCmd *, devResponse *);

#endif

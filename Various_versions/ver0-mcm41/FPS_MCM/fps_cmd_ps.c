#include  "define.h"
#include  "fps_eth.h"
#include "usb_rs485.h"

// ******** Processing the FPS command ******** //
      // NOTE : FPS CONSTRAINT  : COMMAND NO 4 SHALL NOT BE GIVEN
int command_process_fps(parseCMSCmd *FPS_Wrpr_Cmd, devResponse *FPS_MCM_Resp)
{
  int arg_fpscnt=0;
  int fps_def_cnt[4] = { 1300, 6500, 11000, 16500 } ;

  printf("\nEntering Command_Process_FPS");
  memset(out_data,0x00,sizeof(out_data));
 // _f_strcpy(Wrpr_Cmd.cmdelem.cmd_name, "domon");

  if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_401))        // null/domon
  {
  printf("\nfps command: null \n");
  out_data[0] = 8;
  out_data[1] = 0;
  out_data[2] = 0;           // id_code
  out_data[3] = 0;           // NULL = 0
  out_data[4] = 0;
  out_data[5] = 0;           // arg_len
  out_data[6] = 0;
  out_data[7] = chksum(&out_data[0]);     // calculate checksum //
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  return 99;
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_402))
  {
  printf("\nfps command: set turning point \n");

  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ;

  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 1;  // set = 1  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 0; // set turning point = 0 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_403))
  {
  printf("\nfps command: set ramp down count \n");
  out_data[0] = 0x0a;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 1;  // set = 1  // main cmd
  out_data[4] = 0;
  out_data[5] = 2;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 1; // set ramp down count = 1 // sub cmd
  out_data[8] = (int) atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/5 ;  // time count
  out_data[9] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_404))
  {
  printf("\nfps command: set lower RPM Limit \n");  //expected values are like 700 550
  arg_fpscnt = (int) ((15 * 140625)/(160 * atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]))) ;
  /*   teleset-abc
  x = 550; y = 0;        //time interval 0?
  CurrentCmd.Argument[0] = x/5;
  CurrentCmd.Argument[1] = (int) (y*0.2048*x/1800.0);
  */
  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 1;  // set = 1  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 2; // set low rpm limit = 2 // sub cmd
  out_data[8] = arg_fpscnt;
  out_data[9] = 15;
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
 // transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

   else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_405))
  {
  printf("\nfps command: set brake down difference \n");
  out_data[0] = 0x0a;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 1;  // set = 1  // main cmd
  out_data[4] = 0;
  out_data[5] = 2;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 3; // set brake down diff = 3 // sub cmd
  out_data[8] = (int) atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2;   // Encoder Pulses/2
  out_data[9] = chksum(&out_data[0]);     // calculate checksum
 // transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_406))
  {
  printf("\nfps command: set ramp up count \n");
  out_data[0] = 0x0a;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 1;  // set = 1  // main cmd
  out_data[4] = 0;
  out_data[5] = 2;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 4; // set ramp up count = 4 // sub cmd
  out_data[8] = (int) atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/5;  // TMcount/5
  out_data[9] = chksum(&out_data[0]);     // calculate checksum
 // transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_407))
  {
  printf("\nfps command: set stop time count \n");
  out_data[0] = 0x0a;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 1;  // set = 1  // main cmd
  out_data[4] = 0;
  out_data[5] = 2;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 5; // set stop time count = 5 // sub cmd
  out_data[8] = (int)atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/ 5;   // timecount
  out_data[9] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_408))
  {
  printf("\nfps command: set max angle \n");
  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ; // Encoder pulse
  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 1;  // set = 1  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 7; // set max angle = 7 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_409))
  {
  printf("\nfps command: set min angle \n");
  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ; // Encoder pulse
  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 1;  // set = 1  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 8; // set min angle = 8 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_410))
  {
  printf("\nfps command: set max pwm count \n");
  out_data[0] = 0x0a;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 1;  // set = 1  // main cmd
  out_data[4] = 0;
  out_data[5] = 2;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 6; // set max pwm count = 6 // sub cmd
  out_data[8] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]);
  out_data[9] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_411))
  {
  printf("\nfps command: read turnning point \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 0; // read_tpoint = 0 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_412))
  {
  printf("\nfps command: read ramp down count \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 1; // read_rampdcnt = 1 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_413))
  {
  printf("\nfps command: read lower rpm limit \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 2; // read_low_rmp = 2 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_414))
  {
  printf("\nfps command: read brake down diff \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 3; // read_Brake_dd = 3 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
 // transmit();
write_read(buff,buf_ptr0,rbuff);  
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_415))
  {
  printf("\nfps command: read ramp up count \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 4; // read_rampupcnt = 4 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_416))
  {
  printf("\nfps command: read stop time count \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 5; // read_stoptimecnt = 5 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_417))
  {
  printf("\nfps command: read Max angle \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 7; // read_MAX_angle = 7 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_418))
  {
  printf("\nfps command: read Min angle \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 8; // read_Min_angle = 8 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_419))
  {
  printf("\nfps command: read Max pwm count \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 6; // read_MAX_pwm_cnt = 6 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_420))
  {
  printf("\nfps command: read version \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 9; // read_version = 9 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_421))
  {
  printf("\nfps command: read UA0 angle \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 2;  // read = 2  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 0x0a; // read_UA0_angle = 10// sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();    
}

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_422))
  {
  printf("\nfps command: run to calibrate \n");
  out_data[0] = 9;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 1;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 0; // run to calibrate = 0 // sub cmd
  out_data[8] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_423))
  {
  printf("\nfps command: free run \n");
  out_data[0] = 0x0a;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 2;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 1; // run free = 1 // sub cmd
  out_data[8] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]);      // 0=270 or 1=-10 direction of fps
  out_data[9] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_424))
  {
  printf("\nfps command: UA0CALIB \n");
  out_data[0] = 0x0a;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 2;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 5; // UA0CALIB=5 // sub cmd
  out_data[8] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]);      // 0=270 or 1=-10 direction of fps
  out_data[9] = chksum(&out_data[0]);     // calculate checksum

  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_425))
  {
  printf("\nfps command run to C preset \n");
  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ; // Encoder pulse
  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 2; // run to preset = 2 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_426))
  {
    printf("\nfps command run to D preset \n");
   // (512*NINT(CPARM(1)))/18 + (8500-(28.44*NINT(CPARM(2))))

  arg_fpscnt = (atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]) * 28.44)+ (8500 - (28.44 * 270));
  // 270 is hardcoded.
  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 2; // run to preset = 2 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
 // out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
 // out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_427))
  {
  printf("\nfps command run C fine tune \n");
  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ; // Encoder pulse
  out_data[0] = 0x0c;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 4;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 3; // run fine = 3 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
  out_data[10] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[2]);     //
  out_data[11] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_428))
  {
  printf("\nfps command run D fine tune \n");
  arg_fpscnt = (atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]) * 28.44)+ (8500 - (28.44 * 270));
  out_data[0] = 0x0c;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 4;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 3; // run fine = 3 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
  out_data[10] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[2]);     //
  out_data[11] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_429))
  {
  printf("\nfps command: reboot \n");
  out_data[0] = 8;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 5;  // reboot = 5  // main cmd
  out_data[4] = 0;
  out_data[5] = 0;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_430))
  {
  printf("\nfps command: stop \n");
  out_data[0] = 8;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 6;  // stop = 6  // main cmd
  out_data[4] = 0;
  out_data[5] = 0;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_431))
  {
  printf("\nfps command: load fps position \n");
  fps_def_cnt[0] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]); // 610
  fps_def_cnt[1] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[1]); // 150
  fps_def_cnt[2] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[2]); // 1420
  fps_def_cnt[3] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[3]); // 325
  //transmit();
  FPS_resp_code = 10;
write_read(buff,buf_ptr0,rbuff);
  //ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_432))
  {
  printf("\nfps command: Move FPS to 150 \n");
  arg_fpscnt = fps_def_cnt[1]/2 ; // Encoder pulse
  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 2; // run to preset = 2 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_433))
  {
  printf("\nfps command: Move FPS to 375 \n");
  arg_fpscnt = fps_def_cnt[3]/2 ; // Encoder pulse
  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 2; // run to preset = 2 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_434))
  {
  printf("\nfps command: Move FPS to 700 \n");
  arg_fpscnt = fps_def_cnt[0]/2 ; // Encoder pulse
  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 2; // run to preset = 2 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_435))
  {
  printf("\nfps command: Move FPS to 1420 \n");
  arg_fpscnt = fps_def_cnt[2]/2 ; // Encoder pulse
  out_data[0] = 0x0b;
  out_data[1] = 0;
  out_data[2] = 0;
  out_data[3] = 3;  // run = 3  // main cmd
  out_data[4] = 0;
  out_data[5] = 3;  // lsb arg length
  out_data[6] = 0;
  out_data[7] = 2; // run to preset = 2 // sub cmd
  out_data[8] = (int ) arg_fpscnt & 0x00ff ;
  out_data[9] = (int ) arg_fpscnt >> 8;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  out_data[10] = chksum(&out_data[0]);     // calculate checksum
  //transmit();
write_read(buff,buf_ptr0,rbuff);
  ser2eth_comm();
  }

  /*else if(!_f_strcmp(FPS_Wrpr_Cmd.cmdelem.cmd_name,Cmd_436))
  {
    //Struct_Init();             // If this func will be called here, values in struct should be refilled in Final_Resp //
    PortInit();
    ADC_Init();
    Set_RTC(FPS_Wrpr_Cmd.cmdelem.timestamp);
  } */

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_437))
  {
    printf("\n\n****************************\n MCM Reset !!\n****************************\n");
    //sock_close(&FPS_Socket);
    exit(0);
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_438))   // setttime will set RTC //
  {
    //Set_RTC(FPS_Wrpr_Cmd->cmdelem.timestamp);
  }

  else
  {
    printf("Nothing Matched, Received : %s",FPS_Wrpr_Cmd->cmdelem.cmd_name);
  }

  return 12;
}


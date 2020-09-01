
#include  "define.h"
#include  "fps_eth.h"
#include "usb_rs485.h"

int fps_def_cnt[4] = { 1300, 6500, 11000, 16500 } ;

/******* Init buffer function ***********************
 * Make buffer sub function
 * convenient for buffer implementation            */
 void InitBuff(char buff[])
{
  int i, length;
  length = buff[1] + (buff[2]<<8);
  for (i=1; i<length; i++) *(buf_ptr0+i) = buff[i]; // First is MCM Address
}

// ******** Processing the FPS command ******** //
      // NOTE : FPS CONSTRAINT  : COMMAND NO 4 SHALL NOT BE GIVEN
int command_process_fps(parseCMSCmd *FPS_Wrpr_Cmd, devResponse *FPS_MCM_Resp)
{
  long arg_fpscnt=0;

  int i, j;
   
  unsigned char buff[12];


   for(j=0;j<sizeof(wbuff.Msg);j++) { wbuff.Msg[j]=0x00; }

   wbuff.Length = 0;

   buf_ptr0 = (char *)&wbuff.Msg ;
   memset(buf_ptr0,0x00,200); 
   memset(buff,0x00,12);
   buf_ptr0[0] = 0xe;
  
  printf("\nEntering Command_Process_FPS");

  if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_400))        // null
  {
  printf("\nfps command: null \n");

  buff[0] = 0;
  buff[1] = 8;
  buff[2] = 0;
  buff[3] = 0;  
  buff[4] = 0;
  buff[5] = 0; 
  buff[6] = 0;
  buff[7] = 0;  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
/*  if(resp_code == 10 )
    {
        if(rbuff.Length > 1 )
        response_decode();
        printf("FPS mcm finished\n");
        //return ;
    }    */
  return 99;
  }
 else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_401))        // domon
  {
  printf("\nfps command: domon \n");
  FPS_resp_msg_cntr=0 ;

  strcpy(FPS_MCM_Resp->resp_data.prmname[FPS_resp_msg_cntr],"time_of_day");

  time_t rawtime = time(NULL);
  struct tm *ptm = localtime(&rawtime);

   printf("The time is: %02d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
   sprintf(time_ar,"%d:%d:%d",ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
   strcpy(FPS_MCM_Resp->resp_data.prmvalue[FPS_resp_msg_cntr++],time_ar);    
   
  buff[0] = 0;
  buff[1] = 8;
  buff[2] = 0;
  buff[3] = 0;  
  buff[4] = 0;
  buff[5] = 0; 
  buff[6] = 0;
  buff[7] = 0;  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  FPS_resp_code=10; 
  return 99; 

  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_402))
  {
  printf("\nfps command: set turning point \n");

  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ;

  buff[0] = 0;
  buff[1] = 0x0b;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 1;  // set = 1  // main cmd
  buff[5] = 0; 
  buff[6] = 3;  // lsb arg length
  buff[7] = 0; 
  buff[8] = 0; // set turning point = 0 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
  buff[10] =(int) arg_fpscnt >> 8; 
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_403))
  {
  printf("\nfps command: set ramp down count \n");
  buff[0] = 0; 
  buff[1] = 0x0a;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 1;  // set = 1  // main cmd
  buff[5] = 0;
  buff[6] = 2;  // lsb arg length
  buff[7] = 0;
  buff[8] = 1; // set ramp down count = 1 // sub cmd
  buff[9] =(int) atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/5 ;  // time count

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
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
  buff[0] = 0;
  buff[1] = 0x0b;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 1;  // set = 1  // main cmd 
  buff[5] = 0;
  buff[6] = 3;  // lsb arg length
  buff[7] = 0;
  buff[8] = 2; // set low rpm limit = 2 // sub cmd
  buff[9] = arg_fpscnt;
  buff[10] =15; 

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

   else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_405))
  {
  printf("\nfps command: set brake down difference \n");
  buff[0] = 0;
  buff[1] = 0x0a;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 1;  // set = 1  // main cmd
  buff[5] = 0; 
  buff[6] = 2;  // lsb arg length
  buff[7] = 0;
  buff[8] = 3; // set brake down diff = 3 // sub cmd
  buff[9] = (int) atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2;   // Encoder Pulses/2
 
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_406))
  {
  printf("\nfps command: set ramp up count \n");
  buff[0] = 0;
  buff[1] = 0x0a;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 1;  // set = 1  // main cmd
  buff[5] = 0;
  buff[6] = 2;  // lsb arg length
  buff[7] = 0;
  buff[8] = 4; // set ramp up count = 4 // sub cmd
  buff[9] = (int) atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/5;  // TMcount/5

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_407))
  {
  printf("\nfps command: set stop time count \n");
  buff[0] = 0; 
  buff[1] = 0x0a;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 1;  // set = 1  // main cmd 
  buff[5] = 0;
  buff[6] = 2;  // lsb arg length
  buff[7] = 0;
  buff[8] = 5; // set stop time count = 5 // sub cmd
  buff[9] = (int)atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/ 5;   // timecount 
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_408))
  {
  printf("\nfps command: set max angle \n");
  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ; // Encoder pulse
  buff[0] = 0;
  buff[1] = 0x0b; 
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 1;  // set = 1  // main cmd
  buff[5] = 0;
  buff[6] = 3;  // lsb arg length
  buff[7] = 0;
  buff[8] = 7; // set max angle = 7 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
  buff[10] =(int) arg_fpscnt >> 8; 
 
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_409))
  {
  printf("\nfps command: set min angle \n");
  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ; // Encoder pulse
  buff[0] = 0;
  buff[1] = 0x0b;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 1;  // set = 1  // main cmd
  buff[5] = 0;
  buff[6] = 3;  // lsb arg length
  buff[7] = 0;
  buff[8] = 8; // set min angle = 8 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
  buff[10] =(int) arg_fpscnt >> 8; 
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_410))
  {
  printf("\nfps command: set max pwm count \n");
  buff[0] = 0;
  buff[1] = 0x0a;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 1;  // set = 1  // main cmd
  buff[5] = 0; 
  buff[6] = 2;  // lsb arg length
  buff[7] = 0;
  buff[8] = 6; // set max pwm count = 6 // sub cmd
  buff[9] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]);

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_411))
  {
  printf("\nfps command: read turnning point \n");
  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 0; // read_tpoint = 0 // sub cmd
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_412))
  {
  printf("\nfps command: read ramp down count \n");
  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 1; // read_rampdcnt = 1 // sub cmd
 
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_413))
  {
  printf("\nfps command: read lower rpm limit \n");
  buff[0] = 0; 
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 2; // read_low_rmp = 2 // sub cmd
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_414))
  {
  printf("\nfps command: read brake down diff \n");
  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 3; // read_Brake_dd = 3 // sub cmd
 
  write_read(buff,buf_ptr0,rbuff);  
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_415))
  {
  printf("\nfps command: read ramp up count \n");
  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 4; // read_rampupcnt = 4 // sub cmd
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_416))
  {
  printf("\nfps command: read stop time count \n");
  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 5; // read_stoptimecnt = 5 // sub cmd
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_417))
  {
  printf("\nfps command: read Max angle \n");
  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0; 
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 7; // read_MAX_angle = 7 // sub cmd
 
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_418))
  {
  printf("\nfps command: read Min angle \n");
  buff[0] = 0;
  buff[1] = 9; 
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 8; // read_Min_angle = 8 // sub cmd
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_419))
  {
  printf("\nfps command: read Max pwm count \n");
  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0; 
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 6; // read_MAX_pwm_cnt = 6 // sub cmd
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_420))
  {
  printf("\nfps command: read version \n");
  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0; 
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 9; // read_version = 9 // sub cmd
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_421))
  {
  printf("\nfps command: read UA0 angle \n");
  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 2;  // read = 2  // main cmd
  buff[5] = 0;
  buff[6] = 1;  // lsb arg length
  buff[7] = 0;
  buff[8] = 0x0a; // read_UA0_angle = 10// sub cmd
 
  write_read(buff,buf_ptr0,rbuff);
  response_decode();    
}

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_422))
  {
  printf("\nfps command: run to calibrate \n");

  buff[0] = 0;
  buff[1] = 9;
  buff[2] = 0;
  buff[3] = 0;  
  buff[4] = 3;      // run = 3  // main cmd
  buff[5] = 0; 
  buff[6] = 1;      // lsb arg length
  buff[7] = 0;     
  buff[8] = 0;      // run to calibrate = 0 // sub cmd
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_423))
  {
  printf("\nfps command: free run \n");
  buff[0] = 0;
  buff[1] = 0x0a;
  buff[2] = 0;
  buff[3] = 0; 
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0;
  buff[6] = 2;  // lsb arg length
  buff[7] = 0; 
  buff[8] = 1; // run free = 1 // sub cmd
  buff[9] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]);      // 0=270 or 1=-10 direction of fps

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_424))
  {
  printf("\nfps command: UA0CALIB \n");
  buff[0] = 0;
  buff[1] = 0x0a;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0;
  buff[6] = 2;  // lsb arg length
  buff[7] = 0;
  buff[8] = 5; // UA0CALIB=5 // sub cmd
  buff[9] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]);      // 0=270 or 1=-10 direction of fps

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_425))
  {
  printf("\nfps command run to C preset \n");
  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ; // Encoder pulse
  buff[0] = 0;
  buff[1] = 0x0b;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0;
  buff[6] = 3;  // lsb arg length
  buff[7] = 0;
  buff[8] = 2; // run to preset = 2 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10] =(int) arg_fpscnt >> 8; 
 
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_426))
  {
    printf("\nfps command run to D preset \n");
   // (512*NINT(CPARM(1)))/18 + (8500-(28.44*NINT(CPARM(2))))

  arg_fpscnt = (atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]) * 28.44)+ (8500 - (28.44 * 270));
  // 270 is hardcoded.
  buff[0] = 0;
  buff[1] = 0x0b;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0;
  buff[6] = 3;  // lsb arg length
  buff[7] = 0;
  buff[8] = 2; // run to preset = 2 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
 // out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
 // out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10] =(int) arg_fpscnt >> 8; 
 
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_427))
  {
  printf("\nfps command run C fine tune \n");
  arg_fpscnt = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0])/2 ; // Encoder pulse
  buff[0] = 0;
  buff[1] = 0x0c;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0; 
  buff[6] = 4;  // lsb arg length
  buff[7] = 0;
  buff[8] = 3; // run fine = 3 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
  buff[10] =(int) arg_fpscnt >> 8;      
  buff[11] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[2]);
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_428))
  {
  printf("\nfps command run D fine tune \n");
  arg_fpscnt = (atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]) * 28.44)+ (8500 - (28.44 * 270));
  buff[0] = 0;
  buff[1] = 0x0c;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0;
  buff[6] = 4;  // lsb arg length
  buff[7] = 0;
  buff[8] = 3; // run fine = 3 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
  buff[10] =(int) arg_fpscnt >> 8;    
  buff[11] =atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[2]); 
  
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_429))
  {
  printf("\nfps command: reboot \n");

  buff[0] = 0;
  buff[1] = 8;
  buff[2] = 0;
  buff[3] = 0;  // reboot = 5  // main cmd
  buff[4] = 5;
  buff[5] = 0;  // lsb arg length
  buff[6] = 0;
  buff[7] = 0; 
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_430))
  {
  printf("\nfps command: stop \n");
  buff[0] = 0;
  buff[1] = 8;
  buff[2] = 0;
  buff[3] = 0;  
  buff[4] = 6;   // stop = 6  // main cmd
  buff[5] = 0;  
  buff[6] = 0;   // lsb arg length
  buff[7] = 0;    

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_431))
  {
  printf("\nfps command: load fps position \n");
  fps_def_cnt[0] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]); // 610
  fps_def_cnt[1] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[1]); // 150
  fps_def_cnt[2] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[2]); // 1420
  fps_def_cnt[3] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[3]); // 325
  
  FPS_resp_code = 10;
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_432))
  {
  printf("\nfps command: Move FPS to 150 \n");
  arg_fpscnt = fps_def_cnt[1]/2 ; // Encoder pulse
  buff[0] = 0;
  buff[1] = 0x0b;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0;
  buff[6] = 3;  // lsb arg length
  buff[7] = 0;
  buff[8] = 2; // run to preset = 2 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
  buff[10] =(int) arg_fpscnt >> 8; 
 
  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_433))
  {
  printf("\nfps command: Move FPS to 375 \n");
  arg_fpscnt = fps_def_cnt[3]/2 ; // Encoder pulse
  buff[0] = 0;
  buff[1] = 0x0b;
  buff[2] = 0; 
  buff[3] = 0;
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0;
  buff[6] = 3;  // lsb arg length
  buff[7] = 0;
  buff[8] = 2; // run to preset = 2 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
  buff[10] =(int) arg_fpscnt >> 8;     

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_434))
  {
  printf("\nfps command: Move FPS to 700 \n");
  arg_fpscnt = fps_def_cnt[0]/2 ; // Encoder pulse
  buff[0] = 0;
  buff[1] = 0x0b;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0;
  buff[6] = 3;  // lsb arg length
  buff[7] = 0; 
  buff[8] = 2; // run to preset = 2 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10]=(int) arg_fpscnt >> 8; 

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_435))
  {
  printf("\nfps command: Move FPS to 1420 \n");
  arg_fpscnt = fps_def_cnt[2]/2 ; // Encoder pulse
  buff[0] = 0;
  buff[1] = 0x0b;
  buff[2] = 0;
  buff[3] = 0;
  buff[4] = 3;  // run = 3  // main cmd
  buff[5] = 0;
  buff[6] = 3;  // lsb arg length
  buff[7] = 0;
  buff[8] = 2; // run to preset = 2 // sub cmd
  buff[9] = (int) arg_fpscnt & 0x00ff ;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10] =(int) arg_fpscnt >> 8; 

  write_read(buff,buf_ptr0,rbuff);
  response_decode();
  }

  /*else if(!_f_strcmp(FPS_Wrpr_Cmd.cmd_elem.cmd_name,Cmd_436))
  {
    //Struct_Init();             // If this func will be called here, values in struct should be refilled in Final_Resp //
    PortInit();
    ADC_Init();
    Set_RTC(FPS_Wrpr_Cmd.cmd_elem.timestamp);
  } */

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_437))
  {
    printf("\n\n****************************\n MCM Reset !!\n****************************\n");
    //sock_close(&FPS_Socket);
    exit(0);
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_438))   // setttime will set RTC //
  {
    //Set_RTC(FPS_Wrpr_Cmd->cmd_elem.timestamp);
  }

  else
  {
    printf("Nothing Matched, Received : %s",FPS_Wrpr_Cmd->cmdelem.cmd_name);
  }

  return 12;
}


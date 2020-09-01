
#include  "define.h"
#include  "fps_eth.h"

int fps_def_cnt[4] = { 752, 4868, 8952, 13122 } ;


/*--------------Add Check Sum function---------------------------------*/
void AppendAdd(unsigned char *buff);

void add_cksum(char buff[]);

void add_cksum(char buff[])
{
 int length, i;
 char CS=0;

 length = buff[1] + (buff[2]<<8);
 for(i=1; i<length; i++) CS += *(buff+i);
 CS = ~CS+1;

 buff[length] = CS; 

}

void AppendAdd(unsigned char *buff)
{
   int i,length;
   int ret=0; int fd_ret;
   memset(rbuff.Msg,0x00,200);
   resp_fd =0;
    resp_fd = open(respfps,O_RDONLY|O_NONBLOCK); 
     if(resp_fd == -1)
     {
       printf("Problem in Opening RESPONSE FPS FIFO \n");
       printf("Unable to create RESPONSE  FPSFIFO fifo; errno=%d\n",errno);
     }
      else
     {
       printf("RESPONSE FPS FIFO ID in AppendADD ====> %d\n",resp_fd);
     }
     buff[0]=0xe;
    length = buff[1] + (buff[2]<<8);
    add_cksum(buff);  
  /*  printf("FPS FIFO ID in Command_process() ====> %d\n",fps_fd);
    printf("RESPONSE FPS FIFO ID in Command_process() ====> %d\n",resp_fd);*/
    
    write(fps_fd, buff, (length+1));
    //sleep(1); 
     usleep(300000);
    ret=read(resp_fd,rbuff.Msg,100);
   //printf("############# Read Call returned %d\n @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",ret);
   rbuff.Length = rbuff.Msg[1] + (rbuff.Msg[2]<<8);
   rbuff.Length+=1;
   printf("############# Read Call returned %d\n And Rbuff Length is %d\n",ret,rbuff.Length);
  /*  for(i=0; i<rbuff.Length; i++)
   {
     printf("Response PKT in AppendedAdd ====>0x%02X\n",(unsigned char)rbuff.Msg[i]);}*/
      if(rbuff.Length > 8)
      { 
          resp_code = 10; 
           
          fd_ret= close(resp_fd);
           if(fd_ret == -1)
           {
             printf("Unable to Close RESPONSE  FPSFIFO fifo; errno=%d\n",errno); 
           }
           else
            { printf("Successfully close in IF LOOP\n");  }
              response_decode();
       } 
       else
        {  resp_code = 11;
           printf("FPS mcm finished\n");
           fd_ret= close(resp_fd);
           if(fd_ret == -1)
           {
             printf("Unable to Close RESPONSE  FPSFIFO fifo; errno=%d\n",errno); 
           }
           else
            { printf("Successfully close in ELSE LOOP\n");  } 
             
       }
   	 
   /* for (i=0; i<(length+1); i++)
   printf("serial PKT in LOW_Level ====>0x%02X\n",(unsigned char)buff[i]);
   */	 
} 

// ******** Processing the FPS command ******** //
      // NOTE : FPS CONSTRAINT  : COMMAND NO 4 SHALL NOT BE GIVEN
int command_process_fps(parseCMSCmd *FPS_Wrpr_Cmd, devResponse *FPS_MCM_Resp)
{
  int arg_fpscnt=0;

  int i, j;
   int length;
  char buff[12];

  /*    fps_fd = open(fpsfifo,O_WRONLY|O_NONBLOCK); 
      if(fps_fd)
      {
         printf("Opened FPS FIFO Successfully\n"); 
      } */

   
   for(j=0;j<sizeof(wbuff.Msg);j++) { wbuff.Msg[j]=0x00; }

   wbuff.Length = 0;

   buf_ptr0 = (char *)&wbuff.Msg ;
   memset(buf_ptr0,0x00,200); 
   memset(buff,0x00,12);
  // buf_ptr0[0] = 0xe;
  printf("\nEntering Command_Process_FPS");

  if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_400))        // null/domon
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

  AppendAdd(buff);
  return 99;
  }
 else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_401))        // null/domon
  {
  printf("\nfps command: domon \n");

  buff[0] = 0;
  buff[1] = 8;
  buff[2] = 0;
  buff[3] = 0;  
  buff[4] = 0;
  buff[5] = 0; 
  buff[6] = 0;
  buff[7] = 0;  
  
  AppendAdd(buff); 
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
  buff[10] =(int ) arg_fpscnt >> 8; 
   AppendAdd(buff);
 
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
   AppendAdd(buff);
  
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
  AppendAdd(buff);
 
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
 
 AppendAdd(buff);
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

  AppendAdd(buff);
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
  
  AppendAdd(buff);
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
  buff[10] =(int ) arg_fpscnt >> 8; 
 
 AppendAdd(buff);
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
  buff[10] =(int ) arg_fpscnt >> 8; 
  
 AppendAdd(buff);
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

  AppendAdd(buff);
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
  
 AppendAdd(buff);
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
 
  AppendAdd(buff);
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
  
  AppendAdd(buff);
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
 
 AppendAdd(buff);
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
  
AppendAdd(buff);
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
  
  AppendAdd(buff);
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
 
 AppendAdd(buff);
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
  
  AppendAdd(buff);
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
  
  AppendAdd(buff);
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
  
  AppendAdd(buff);
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
 
  AppendAdd(buff);
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
  AppendAdd(buff);
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

  AppendAdd(buff);
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

  AppendAdd(buff);
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10] =(int ) arg_fpscnt >> 8; 
 
  AppendAdd(buff);
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
 // out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
 // out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10] =(int ) arg_fpscnt >> 8; 
 
 AppendAdd(buff);
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
  buff[10] =(int ) arg_fpscnt >> 8;      
  buff[11] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[2]);
  
   AppendAdd(buff);
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
  buff[10] =(int ) arg_fpscnt >> 8;    
  buff[11] =atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[2]); 
  
  AppendAdd(buff);
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
  AppendAdd(buff);
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

  AppendAdd(buff);
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_431))
  {
  printf("\nfps command: load fps position \n");
  fps_def_cnt[0] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[0]); // 610
  fps_def_cnt[1] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[1]); // 150
  fps_def_cnt[2] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[2]); // 1420
  fps_def_cnt[3] = atoi(FPS_Wrpr_Cmd->cmd_data.prmvalue[3]); // 325
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("######ENCODER COUNT are 610 => %d 150=> %d  1420 => %d 325 => %d\n",fps_def_cnt[0],fps_def_cnt[1], fps_def_cnt[2], fps_def_cnt[3]);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
  FPS_resp_code = 10;
  // AppendAdd(buff);
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_432))
  {
  printf("\nfps command: Move FPS to 150 \n");
   printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("######ENCODER COUNT are 150=> %d\n",fps_def_cnt[1]);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10] =(int ) arg_fpscnt >> 8; 
 
  AppendAdd(buff);
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_433))
  {
  printf("\nfps command: Move FPS to 375 \n");
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("######ENCODER COUNT are 375=> %d\n",fps_def_cnt[3]);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10] =(int ) arg_fpscnt >> 8;     

 AppendAdd(buff);
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_434))
  {
  printf("\nfps command: Move FPS to 700 \n");
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("######ENCODER COUNT are 700=> %d\n",fps_def_cnt[0]);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10]=(int ) arg_fpscnt >> 8; 

 AppendAdd(buff);
  }

  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_435))
  {
  printf("\nfps command: Move FPS to 1420 \n");
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("######ENCODER COUNT are 1420=> %d\n",fps_def_cnt[2]);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
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
  buff[9] = (int ) arg_fpscnt & 0x00ff ;
//  out_data[8] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[0]);      // tgt angle
//  out_data[9] = _f_atoi(Wrpr_Cmd.cmd_data.prmvalue[1]);      // tgt angle
  buff[10] =(int ) arg_fpscnt >> 8; 

  AppendAdd(buff);
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


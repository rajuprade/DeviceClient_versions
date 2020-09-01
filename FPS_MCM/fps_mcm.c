

#include  "define.h"
#include  "usb_rs485.h"

void response_decode()
{
 int i,temp,j, l;
 int bytecntr_in;  //mcm cmd len including mcm addr byte
 int maxpktlen;    //holds maximum mcm cmd len
 char watchdogflag, overcurrentflag;
 long int Rpm;
 unsigned long EncCount=0; // Changed by RU on 18/04/19

 char *par_name[TVAR] = { "current_encoder", "target_encoder","current_angle","target_angle", "current_rpm",
 "mode", "fps_version", "ramp_dntime", "ramp_uptime","low_rpm_lmt", "low_rpm_time",
 "brk_cnt_diff", "step_slew_timecnt","fine_tune_pwm", "max_pwm", "max_angle","min_angle",
 "ua0_angle","over_current","commerrors","cmd_fail","timeout_cnt","chksum_cnt","reset_cnt",
 "fps_control_info", "fps_motor_temp","motor_curr","domon_stat", "turning_pt","stop_cnt" };

 bytecntr_in = 0;
 FPS_resp_msg_cntr = 0;
 maxpktlen = 0;
 l = 0;
 
 bytecntr_in=rbuff.Length;
 
 memset(in_d,0x00,sizeof(in_d));
 
// ------------ Read Packet --------------//
memcpy(in_d,rbuff.Msg,sizeof(rbuff.Msg));
printf("\nbytecntr_in: %d\n", bytecntr_in);

printf("Data inside in_d: \n");
for(i=0;i<bytecntr_in;i++)
{
printf("[%2d] : %x \n", i, in_d[i]);
}

i=0;
while(in_d[i]!=0x0e)
{
i=i+1;
if(i>5)
break;
}

   if(bytecntr_in <= 0 )
   {
   strcpy(dev_Resp.resp_data.prmname[FPS_resp_msg_cntr],"cmdstatus");
   strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"No Answer from FPS");
   printf("\nNo Answer from FPS");
   FPS_resp_msg_cntr++;
   FPS_resp_code = 11;
   return;
   }
  //---------- cmdstatus ------------------------//
 strcpy(dev_Resp.resp_data.prmname[FPS_resp_msg_cntr],"cmdstatus");

if( (bytecntr_in > 50)|| (bytecntr_in < 0x0B) )  //null resp + addr byte = 0B
{//printf("pkt size error. \n");
 strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Packet Size error");
 printf("\nPacket Size error");
  FPS_resp_msg_cntr++;
  FPS_resp_code = 11;
 return ;
}   // exit if pkt_len is more than 50 bytes    

if(in_d[i+0] != 0x0e)
//if(rbuff.Msg[0] != 0x0e)
{
printf("NOT FPS 14 Packet.\n");
 sprintf(dev_Resp.resp_data.prmname[FPS_resp_msg_cntr],"NOT FPS 14 Packet %d - %s" ,bytecntr_in,in_d);
 FPS_resp_msg_cntr++;
 FPS_resp_code = 11;
 return ;
}   // exit if mcm addr is not 0e or 14 dec
printf(" FPS 14 Packet.\n");
// check status byte.

 if(in_d[i+4] == 0x00)
   strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Exec OK");
 if(in_d[i+4] & 0x01)
   strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"FPS detected timeout");
 if(in_d[i+4] & 0x02)
   strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"FPS detected CS error");
 if(in_d[i+4] & 0x04)
   strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"FPS received unknown command");
 if(in_d[i+4] & 0x08)
   strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"FPS received unknown command");
 if(in_d[i+4] & 0x10)
   strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"FPS received correct Pkt but rejected cmd");
 if(in_d[i+4] & 0x20)
  {
    if(!watchdogflag)
    {
     strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"FPS watchdog reset");
    watchdogflag = 1;
    }
    else
    {
    watchdogflag = 0;
    }
  }
 if(in_d[i+i+4] & 0x40)
 {
    if(!overcurrentflag)
    {
     strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"FPS over current");
    overcurrentflag = 1;
    }
    else
    {
    overcurrentflag = 0;
    }
  }
  FPS_resp_msg_cntr++; // cmdstatus over

 if( in_d[i+5] >= 0x01)   //1st_of_lp
  {
      // --------------fps_state --------------------------------//
       strcpy(dev_Resp.resp_data.prmname[FPS_resp_msg_cntr],"fps_state");

    if( (in_d[i+6] != 7) ||(in_d[i+7] != 0) ||(in_d[i+8] != 4) )  // in_d[i+6+7] packet length, in_d[i+8] is fixed 4
    {
      strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Resp Logical Pkt error");
     FPS_resp_msg_cntr++;
     FPS_resp_code = 11;
     return ;
    }
    else  
    {

     temp = in_d[i+9];
     switch(temp)
     {
     case(0x42): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"280 Limit Hit");
                 break;
     case(0x43): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"-10 Limit Hit");
                 break;
     }

     temp &= 0xfe;

     switch(temp)
     {
     case(0x00): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Feed Calibrated and Idle");
                 break;
     case(0x40): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Feed uncalibrated");
                 break;
     case(0x08): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Busy - Preset Run");
                 break;
     case(0x10): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Busy - Free Run");
                 break;
     case(0x4a): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Busy - Passworded Run");
                 break;
     case(0xc0): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Busy - In Calibration");
                 break;

     case(0x28): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Jammed - Preset Run");
                 break;
     case(0x30): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Jammed - Free Run");
                 break;
     case(0xe0): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Jammed - In Calibration");
                 break;
     case(0x62): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Jammed - Passworded");
                 break;
     case(0x20): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Jammed ");
                 break;
     case(0xff): strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"Ua0 OFFSET Processing");
                 break;
     } //case ends
        FPS_resp_msg_cntr++;
        //------------ fps_state end here -----------------------//

             //calculate EncCount
     EncCount = ( in_d[i+10] + (in_d[i+11]<<8) ) *2;
     sprintf(dev_Resp.resp_data.prmname[FPS_resp_msg_cntr], "current_encoder");
     sprintf(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++], "%ld", EncCount);

//calculate Rpm
     Rpm = (in_d[i+12]*5*60)/2048*300;
     sprintf(dev_Resp.resp_data.prmname[FPS_resp_msg_cntr], "current_rpm");
     sprintf(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++], "%ld", Rpm);
    }//else
  }//if 1st lp ends  in_d[i+5]


 if(in_d[i+5] == 0x02)   //2nd_of_lp = main cmd, sub cmd,
  {
    // ---------   fpscmd ---------------------------//
   sprintf(dev_Resp.resp_data.prmname[FPS_resp_msg_cntr], "fpscmd");

   if((in_d[i+15] >6)||(in_d[i+15] < 0))
    {
    strcpy(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"unknown cmd");
    FPS_resp_msg_cntr++;
    FPS_resp_code = 11;
    return;
    }

    // commented part is written by CPK.
   /*if(in_d[i+15] == 0)  // main cmd = NULL
    {
     sprintf(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++], "domon"); // NULL changed to domon

    _f_strcpy(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr],"FPS Status");

     if(!(in_d[i+16] & 0x01))
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"mode MAN ");
     else
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"mode AUTO ");

     if(!(in_d[i+16] & 0x02))
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"rgso OFF ");
     else
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"rgso ON ");

     if(!(in_d[i+16] & 0x04))
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"dir 270 ");
     else
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"dir -10 ");

     if(!(in_d[i+16] & 0x08))
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"fv OFF ");
     else
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"fv ON ");

     if(!(in_d[i+16] & 0x10))
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"t80l OK ");
     else
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"t80l HIT ");

     if(!(in_d[i+16] & 0x20))
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"t70l OK ");
     else
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"t70l HIT ");

     if(!(in_d[i+16] & 0x40))
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"n51 OK ");
     else
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"n51 HIT ");

     if(!(in_d[i+16] & 0x80))
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"n101 OK ");
     else
     _f_strcpy(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++],"n101 HIT ");

//motor current in_d[i+17]
     sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr], "Motor cur");
     sprintf(FPS_MCM_Resp.resp_data.prmvalue[FPS_resp_msg_cntr++], "%.2f", (float)(in_d[i+17]/12.5));
// to be implemented
//Brake current in_d[i+18]
     sprintf(FPS_MCM_Resp.message[FPS_resp_msg_cntr++], "Brake cur %f", (float)(in_d[i+18]/127.5);
//CPU 5vdc      in_d[i+19]
     sprintf(FPS_MCM_Resp.message[FPS_resp_msg_cntr++], "CPU 5vdc %f", (float)(in_d[i+19]/51.0);
// Env 5vdc     in_d[i+20]
     sprintf(FPS_MCM_Resp.message[FPS_resp_msg_cntr++], "Enc 5vdc %f", (float)(in_d[i+10]/51.0);

     }*///if in_d[i+15] ends here

  }//  2nd_of_lp end here.



     // NOTE READ and SET Sub-Command IDs Assumed identical

   if(in_d[i+15] == 1)  // main cmd = SET
    {
       sprintf(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr], "SET");
    }

   if(in_d[i+15] == 2)  // main cmd = READ
    {
       sprintf(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr], "READ");
    }

   if ( in_d[i+15] == 1 || in_d[i+15] == 2 )
   {
     switch(in_d[i+16])
     {
          case 0:
          temp = 2*(in_d[i+17]+(in_d[i+18]<<8));
          strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_TNG_PNT");
          sprintf(par_value[TNG_PT],"%d",temp);
          // sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++], "Set Turning Point, target: %d", temp);
          break;

          case 1:
          strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_RMP_DNCNT");
          sprintf(par_value[RAMP_DNTIME],"%d",in_d[i+17]*5);
          //sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++], "Set Ramp Down Count, Slope: %d", in_d[i+17]*5);
          break;

          case 2:
          strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_LRPM_LMT");
          temp = (int) (in_d[i+18]/0.2048*1800.0/(in_d[i+17]*5) );
          sprintf(par_value[LOW_RPM_LMT],"%d",temp);
          // sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++], "Set Lower Ramp Limit, %d int %d", temp, in_d[i+17]*5);
          break;

          case 3:
          strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_BRKCNT_DIFF");
          sprintf(par_value[BRK_CNT_DIFF],"%d",(int) in_d[i+17]*2);
          //sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++], "Set Break Count Diff, %d ",(int) in_d[i+17]*2);
          break;

          case 4:
          strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_RMP_UPCNT");
          sprintf(par_value[RAMP_UPTIME],"%d",in_d[i+17]*5);
         // sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++], "Set Ramp up Count, %d ", in_d[i+17]*5);
          break;

          case 5:
          strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_STOP_CNT");
          sprintf(par_value[STOP_CNT],"%d",in_d[i+17]*5);
          //sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++], "Set Stop Count, %d ", in_d[i+17]*5);
          break;

          case 6:
          strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_MAX_PWM");
            sprintf(par_value[MAX_PWM],"%d",in_d[i+17]);
          //sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++], "Set Max PWM Count, %x ", in_d[i+17]);
          break;

          case 7:
          strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_MAX_ANG");
          temp = in_d[i+17]+(in_d[i+18]<<8);
          sprintf(par_value[MAX_ANG],"%d",temp * 2);
          //sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++], "Set Max Angle, %d ", temp*2);
          break;

          case 8:
          strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_MIN_ANG");
          temp = in_d[i+17]+(in_d[i+18]<<8);
          sprintf(par_value[MIN_ANG],"%d",temp * 2);
         // sprintf(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++], "Set Min Angle, %d ", temp*2);
           break;

          case 9:
           strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_VERSION");
           sprintf(par_value[FPS_VER],"%f",(float) in_d[i+17]/10);

          case 10 :
           strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_UA0_ANG");
           temp = 2*(in_d[i+17]+(in_d[i+18]<<8));
           sprintf(par_value[UA0_ANG],"%d",temp);

     } // case ends here
    /* TBD ---  10/9/2015
     if(in_d[i+16] == 9)
      _f_strcpy(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++],"Write Addr ");    // case 9
     else if(in_d[i+16] == 10)
      _f_strcpy(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++],"Load Count ");    //case 10
    }// if in_d[i+15] == 1 ie SET cmd ends
     */
  } else if(in_d[i+15] == 3)  // main cmd = RUN
   {
    sprintf(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr], "Run");
    switch(in_d[i+16])
    {
          case 0: strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_CALIBRATE");break;
         // _f_strcpy(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++],"Run to calibrate"); break;
          case 1: strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_FREE");break;
         // _f_strcpy(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++],"Run Free"); break;
          case 2: strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_TO_RESET");break;
         // _f_strcpy(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++],"Run to Reset"); break;
          case 3: strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_TO_FINETUNE");  break;
         // _f_strcpy(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++],"Run to Fine Tune"); break;
          case 4: strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_PASSWORD");  break;
         // _f_strcpy(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++],"Run Passworded"); break;
          case 5: strcat( dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"_TO_UA0");  break;
         // _f_strcpy(FPS_MCM_Resp.resp_data.prmname[FPS_resp_msg_cntr++],"Run to Ua0"); break;
      }
   } else if(in_d[i+15] == 5)  // main cmd = reboot
   {
     sprintf(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr], "Reboot");
   } else if(in_d[i+15] == 6)  // main cmd = stop
   {
    // if in_d[i+15] == 5 ie reboot cmd ends
      sprintf(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr], "Stop");
   }// if in_d[i+15] == 6 ie stop cmd ends
   FPS_resp_msg_cntr++;
   //---------------fpscmd end


   for (i = 0;i < TVAR; i++)
   {
      sprintf(dev_Resp.resp_data.prmname[FPS_resp_msg_cntr],"%s",par_name[i]);
      sprintf(dev_Resp.resp_data.prmvalue[FPS_resp_msg_cntr],"%s",par_value[i]);
      FPS_resp_msg_cntr++;
   }

/*for(i=0;i<FPS_resp_msg_cntr;i++)
printf("\n[%2d] : %s",i, FPS_MCM_Resp.resp_data.prmvalue[i]); */

printf("end of serial transmission... ser2eth \n\n");
FPS_resp_code = 10;

}




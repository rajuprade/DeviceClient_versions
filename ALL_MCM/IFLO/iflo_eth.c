/*   Forming intermediate response for all system commands  
     Before forming, Command Validation function should be called,
     Which will validate the command and System.
*/
#include "define.h"
#include "iflo_mcm.h"
#include "iflo_eth.h"
int form_inter_resp(parseCMSCmd *Wrpr_Cmd, devResponse *MCM_Resp)
{
  int loop;

  printf("\nEntering form_inter_resp");

  MCM_Resp->code = cmd_validation(Wrpr_Cmd,MCM_Resp);
  MCM_Resp->event = INTERMEDIATE;
  MCM_Resp->MCM_alarm.level=1;
  switch(MCM_Resp->code)
  {
    case 10 :  strcpy(MCM_Resp->message,"Command received successfully");
               printf("\nCommand received successfully");
               break;

    case 12 :  strcpy(MCM_Resp->message,"Command Syntax Error");
               printf("\nCommand Syntax Error");
               break;
    case 13 :  strcpy(MCM_Resp->message,"Command Incomplete");
               printf("\nCommand Incomplete");
               break;
    case 14 :  strcpy(MCM_Resp->message,"Command Invalid");
               printf("\nCommand Invalid");
               break;
    case 100:  strcpy(MCM_Resp->message,"Error Unknown");
               printf("\nError Unknown");
               break;
    default :  break;
  };

 // ******** Write in to Tx_BasicFlds *********** //
  if ( MCM_Resp->code != 10 )
  MCM_Resp->event = FINAL;

  MCM_Resp->resp_elem.seq = Wrpr_Cmd->cmdelem.seq;
  MCM_Resp->resp_elem.version = Wrpr_Cmd->cmdelem.version;
  strcpy(MCM_Resp->resp_elem.subsysid,Wrpr_Cmd->cmdelem.subsysid);
  strcpy(MCM_Resp->resp_elem.cmd_id,Wrpr_Cmd->cmdelem.cmd_id);
  strcpy(MCM_Resp->resp_elem.cmd_name,Wrpr_Cmd->cmdelem.cmd_name);
  strcpy(MCM_Resp->resp_elem.timestamp,Wrpr_Cmd->cmdelem.timestamp);
  MCM_Resp->resp_elem.priority = Wrpr_Cmd->cmdelem.priority;
  MCM_Resp->resp_elem.timeout = Wrpr_Cmd->cmdelem.timeout;

  // ******** Write in to Tx_DataPkt *********** //
  MCM_Resp->resp_data.numpkt = 0;
  for(loop=0; loop<MAXDATA; loop++)
   {
     strcpy(MCM_Resp->resp_data.prmname[loop],"\0");
     strcpy(MCM_Resp->resp_data.prmvalue[loop],"\0");
   }
  // ******** Write in to Tx_alarm *********** //
  MCM_Resp->MCM_alarm.id = 0;
  MCM_Resp->MCM_alarm.level = 0;
  strcpy(MCM_Resp->MCM_alarm.description,"\0");

  return MCM_Resp->code;
}



// ********* This function will validate command against various checks. ********* //
// **** Syntax error, Invalid, Incomplete command, Other error will be checked **** //

int cmd_validation(parseCMSCmd *Wrpr_Cmd, devResponse *MCM_Resp)
 {
   int cmpl=0, i, j;
   /* char tmpstr[256];

    strcpy(tmpstr,Wrpr_Cmd->cmdelem.cmd_name);
    strcat(tmpstr, Wrpr_Cmd->cmdelem.subsysid);
    printf("\n #### Command received: %s \n", tmpstr);

    strcpy(tmpstr, Wrpr_Cmd->cmdelem.timestamp); printf("time: %s \n", tmpstr);
    strcpy(tmpstr, Wrpr_Cmd->cmdelem.cmd_id);  printf("cmdId: %s \n", tmpstr);

  //  printf("NumPkt %d \n", Wrpr_Cmd->cmd_data.numpkt);

    for (i=0;i< Wrpr_Cmd->cmd_data.numpkt;i++)
    {
     strcpy(tmpstr,Wrpr_Cmd->cmd_data.prmname[i]); //printf("Name %s ", tmpstr);
     strcpy(tmpstr,Wrpr_Cmd->cmd_data.prmvalue[i]); //printf("value %s \n", tmpstr);
    } */

  

      if(!strcmp(Wrpr_Cmd->cmdelem.subsysid,SYSTEM_5))
       {
         printf("\nSubsystem validated");

          if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_500))
          {
           return 10;
          }
         else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_501))
          {
           
           return 10;
          }
          else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_502))
          {
           return 10;
          }  
         else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_503))
          {
           return 10;
          }        
          else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_504))
          {
           return 10;
          } 
           else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_505))
          {
           return 10;
          } 
           else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_506))
          {
           return 10;
          } 
           else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_507))
          {
           return 10;
          }
            else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_508))
          {
           return 10;
          }  
            else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_509))
          {
           return 10;
          }  
            else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_510))
          {
           return 10;
          }  
            else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_511))
          {
           return 10;
          }  
            else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_512))
          {
           return 10;
          } 
            else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_513))
          {
           return 10;
          } 
         else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_514))
          {
           return 10;
          }                         
         else
          {
           return 12;     // Incase of Syntax Error in command //
          }
       }
      else
       {
        return 14;
       }

 }


/* Final Response function will in turn call FE_command process function */

// ******** Forming final response ******** //

void IFLO_form_final_resp(parseCMSCmd *Wrpr_Cmd, devResponse *MCM_Resp)
{
  printf("In IFLO FORM FINAL RESPONSE &&&&&&&& =>  =>\n");
  MCM_Resp->event = command_process_IFLO(Wrpr_Cmd,MCM_Resp);
  MCM_Resp->code = resp_code; // previously 'code' was here?
  MCM_Resp->resp_data.numpkt = resp_msg_cntr;

  printf("\n ===> form_final_resp  :: EVENT %d CODE %d NUMPKT %d \n", MCM_Resp->event, MCM_Resp->code, MCM_Resp->resp_data.numpkt);
}

/* Forming Alarm Response */
/* void form_alarm(int Alrm_id, int Alrm_lvl)
{
  MCM_Resp.code = 10;
  MCM_Resp.event = 100;

  MCM_Resp.MCM_alarm.id = Alrm_id;
  MCM_Resp.MCM_alarm.level = Alrm_lvl;
  strcpy(Wrpr_Cmd.cmdelem.cmd_name,"domon");
  command_process_FE();
  //sock_write(&Socket, &MCM_Resp, MCM_Resp_Size);

} */


/* Fill Set Data function is called to fill the setting parameter in DOMON response structure */
// fillSetPara will take later---> RAJ
void fillSetpara(parseCMSCmd *Wrpr_Cmd, devResponse *MCM_Resp)
{

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"ILO_CH1");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%d",ifloSet.lo[0]);
       resp_msg_cntr++;

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"ILO_CH2");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%d",ifloSet.lo[1]);
       resp_msg_cntr++;

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"IF_BAND_CH1");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%d", ifloSet.if_band[0] );
       resp_msg_cntr++;

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"IF_BAND_CH2");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%d", ifloSet.if_band[1] );
       resp_msg_cntr++;

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"IF_PA_CH1");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%f", ifloSet.if_pa[0] );
       resp_msg_cntr++;

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"IF_PA_CH2");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%f", ifloSet.if_pa[1] );
       resp_msg_cntr++;
       
        strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"IF_PG_CH1");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%d", ifloSet.if_pg[0] );
       resp_msg_cntr++;

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"IF_PG_CH2");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%d", ifloSet.if_pg[1] );
       resp_msg_cntr++;
    
        strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"IF_ALC_CH1");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%d", ifloSet.if_alc[0] );
       resp_msg_cntr++;

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"IF_ALC_CH2");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%d", ifloSet.if_alc[1] );
       resp_msg_cntr++;
}


// ******** Processing the FE commands ******** //
int command_process_IFLO(parseCMSCmd *Wrpr_Cmd, devResponse *MCM_Resp)
{
  int arg_fpscnt=0;
  int i, j ;
  char *fevar[] =  { "ilo_ch1","ilo_ch2","ifbw_ch1" ,"ifbw_ch2" ,"pre_attn_ch1","pre_attn_ch2","post_gain_ch1" ,"post_gain_ch2" ,"ifalc_ch1" ,
                    "ifalc_ch2","ofcatten_ch1","ofcatten_ch2","fe_walsh_sw" ,"fe_walsh_grp" ,"fe_ngcycle" ,"rfcm_sw", "noisefreq","walshpattern","walshfreq"};
  typedef enum { ILO_CH1,ILO_CH2,IFBW_CH1,IFBW_CH2, PRE_ATTN_CH1,PRE_ATTN_CH2,POST_GAIN_CH1,POST_GAIN_CH2,IFALC_CH1,IFALC_CH2,OFCATTEN_CH1,OFCATTEN_CH2,FE_WALSH_SW, FE_WALSH_GRP, FE_NG_CYCLE, RFCM_SW, NOISEFREQ,WALSHPATTERN,WALSHFREQ,TSET};

//  printf("\nEntering Command_Process_IFLO");


  resp_msg_cntr = 0; // Reset the response
  resp_code = 0;
 // bzero(&ifloSet,sizeof(ifloSet));

  if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_500))      // SET_LO
  {
    ifloSet.cmdCode = 0; printf("In Command Process LOIF => SET LO =>\n");
  }

  else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_501))      // SET_IF
  {      
     ifloSet.cmdCode = 1;  printf("In Command Process LOIF => SET IF =>\n");   
  }

  else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_502)) // SET_OFC_ATTN
  {

    ifloSet.cmdCode = 2;
    printf("In Command Process LOIF => SET OFC ATTN =>\n");
  }

  else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_503))           // domon
  {

  // resp_msg_cntr=0 ;

   strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"time_of_day");

   time_t rawtime = time(NULL);
   struct tm *ptm = localtime(&rawtime);

    printf("The time is: %02d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    sprintf(time_ar,"%d:%d:%d",ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    strcpy(MCM_Resp->resp_data.prmvalue[resp_msg_cntr++],time_ar);
   
  ifloSet.cmdCode = 3; // fecbCtrlCmd();
  ifloCtrlCmd(Wrpr_Cmd);
  fillSetpara(Wrpr_Cmd,MCM_Resp);
  resp_code=10;    // domon successful for timebeing
  return 99;

  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_504)) // Servo System Reset
  {

    ifloSet.cmdCode = 4;
    printf("In Command Process LOIF => SERVO RESET =>\n");
  }
  else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_505)) // FPS SYSTEM RESET
  {

    ifloSet.cmdCode = 5;
    printf("In Command Process LOIF => FPS RESET =>\n");
  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_506)) // Complete SIGCON SYSTEM
  {

    ifloSet.cmdCode = 6;
    printf("In Command Process LOIF => SET SIGCON SYSTEM =>\n");
  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_507)) // SET IF ATTENUATION 
  {

    ifloSet.cmdCode = 7;
    printf("In Command Process LOIF => SET IF ATTENUATION  =>\n");
  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_508)) // SET WALSH pattern ( 0,7) 
  {

    ifloSet.cmdCode = 8;
    
  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_509))  // set walsh group 
  {

    ifloSet.cmdCode = 9;
    
  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_510)) // MCM 5 on /OFF 
  {

    ifloSet.cmdCode = 10;
    
  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_511))  // WALSH FREQUENCY (0,3) 
  {

    ifloSet.cmdCode = 11;
    
  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_512)) // NOISE FREQUENCY (1,3) 
  {

    ifloSet.cmdCode = 12;
    
  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_513))  // NG Duty Cycle 
  {

    ifloSet.cmdCode = 13;
    
  }
   else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_514))  // Set walsh 
  {

    ifloSet.cmdCode = 14;
    
  }
  else
  {
    printf("Nothing Matched, Received : %s",Wrpr_Cmd->cmdelem.cmd_name);
    return 12;
  }

//#ifdef DEBUG
  for (j = 0 ; j < Wrpr_Cmd->cmd_data.numpkt;j ++)
  {
    printf(" Parameter name %s\n",Wrpr_Cmd->cmd_data.prmname[j]);
    printf(" Parameter name %s\n",Wrpr_Cmd->cmd_data.prmvalue[j]);
  }
//#endif    
  for (j = 0 ; j < Wrpr_Cmd->cmd_data.numpkt;j++)
  { 
       for (i = 0 ; i < TSET; i++ )
      {
        
          if(! strcasecmp(Wrpr_Cmd->cmd_data.prmname[j], fevar[i]))
        { 
          
        switch(i)
        {
             case ILO_CH1 :              
                                     ifloSet.lo[0] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                      printf("In SWITCh Case LO1 ======== =>%d\n",ifloSet.lo[0]);
                                     break;

             case ILO_CH2 :            
                                    ifloSet.lo[1] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]); 
                                    printf("In SWITCh Case LO2 ======== =>%d\n",ifloSet.lo[1]);
                                     break;

             case IFBW_CH1   :  ifloSet.if_band[0] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                  printf("In SWITCh Case IFBW_CH1 ======== =>%d\n",ifloSet.if_band[0]);  
                                     break;

             case IFBW_CH2   :  ifloSet.if_band[1] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                     printf("In SWITCh Case IFBW_CH1 ======== =>%d\n",ifloSet.if_band[1]); 
                                     break;

             case PRE_ATTN_CH1    :  ifloSet.if_pa[0] = (float) atof(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                     printf("In SWITCh Case PRE_ATTN_CH1 ======== =>%f\n",ifloSet.if_pa[0]); 
                                     break;

             case PRE_ATTN_CH2          :  ifloSet.if_pa[1] = (float) atof(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    printf("In SWITCh Case PRE_ATTN_CH2 ======== =>%f\n",ifloSet.if_pa[1]);
                                     break;

             case POST_GAIN_CH1     :  ifloSet.if_pg[0] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                     printf("In SWITCh Case POST_GAIN_CH1 ======== =>%d\n",ifloSet.if_pg[0]);
                                     break;

             case POST_GAIN_CH2      :  ifloSet.if_pg[1] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                          printf("In SWITCh Case POST_GAIN_CH2 ======== =>%d\n",ifloSet.if_pg[1]);                           
                                     break;

             case IFALC_CH1     :  ifloSet.if_alc[0]= (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);                               
                                    printf("In SWITCh Case IF_ALC_CH1 ======== =>%d\n",ifloSet.if_alc[0]);
                                     break;

             case IFALC_CH2     :  ifloSet.if_alc[1] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    printf("In SWITCh Case IF_ALC_CH2 ======== =>%d\n",ifloSet.if_alc[1]);
                                     break;
             case OFCATTEN_CH1   : ofc_ch1 = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    printf("In SWITCh Case OFC_CH1 ======== =>%d\n",ofc_ch1);
                                     break;
            case  OFCATTEN_CH2   : ofc_ch2 = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    printf("In SWITCh Case OFC_CH2 ======== =>%d\n",ofc_ch2);
                                     break;
             case FE_WALSH_GRP    :  fecbSet.walshGrp = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                     
                                     break;
             case FE_WALSH_SW     :  fecbSet.walshSW = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                     if( fecbSet.walshSW == 1 ) fecbSet.walshSW =0;  // Enable, -ve logic
                                     else fecbSet.walshSW = 8;  // Disable, -ve logic
                                     
                                     break;
             case FE_NG_CYCLE     :  fecbSet.ngCycle  = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                     switch(fecbSet.ngCycle)
                                     {
                                          case 0 : fecbSet.ngCycle = 0 ; break;
                                          case 25 : fecbSet.ngCycle = 1 ; break;
                                          case 50 : fecbSet.ngCycle = 2 ; break;
                                          case 100 : fecbSet.ngCycle = 3 ; break;
                                     }
                                    
                                     break;

             case RFCM_SW         :  fecbSet.rfcmSW = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    
                                     break;

             

             case NOISEFREQ     :  fecbSet.ngFreq =(int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    printf("######### NOISE FREQUENCY in FECB_ETH is %d\n",fecbSet.ngFreq);
                                    if( fecbSet.ngFreq > 3 || fecbSet.ngFreq  < 0 )
                                    fecbSet.ngFreq = 0;
                                     break;

             case WALSHPATTERN   :  fecbSet.walshPat =(int)atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    printf("######### WALSH PATTERN in FECB_ETH is %d\n",fecbSet.walshPat);
                                    if( fecbSet.walshPat > 7 || fecbSet.walshPat  < 0 )
                                     fecbSet.walshPat = 0;
                                     break;

             case WALSHFREQ  :  fecbSet.walshFreq =(int)atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                 printf("######### WALSH FREQUENCY in FECB_ETH is %d\n",fecbSet.walshFreq);
                                 /*   if( fecbSet.walshFreq > 3 || fecbSet.walshFreq  < 0 )
                                    fecbSet.walshFreq = 0;*/
                                    break;

            
           }
         }
          

        }  
      }
 

 ifloCtrlCmd(Wrpr_Cmd);
  return 12;
}





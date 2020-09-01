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
   char tmpstr[256];

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
    }

  

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
         else if(!strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_503))
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
  resp_msg_cntr = 0 ;
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

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"LO1");
       sprintf(MCM_Resp->resp_data.prmvalue[resp_msg_cntr],"%d",ifloSet.lo[0]);
       resp_msg_cntr++;

       strcpy(MCM_Resp->resp_data.prmname[resp_msg_cntr],"LO2");
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
  char *fevar[] =  { "lo1","lo2","if_band_ch1" ,"if_band_ch2" ,"if_pa_ch1","if_pa_ch2","if_pg_ch1" ,"if_pg_ch2" ,"if_alc_ch1" ,"if_alc_ch2"};
  typedef enum { LO1,LO2,IF_BAND_CH1,IF_BAND_CH2, IF_PA_CH1,IF_PA_CH2,IF_PG_CH1,IF_PG_CH2,IF_ALC_CH1,IF_ALC_CH2,TSET};

//  printf("\nEntering Command_Process_IFLO");


  resp_msg_cntr = 0; // Reset the response
  resp_code = 0;
  bzero(&ifloSet,sizeof(ifloSet));

  if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_500))      // SET_LO
  {
    ifloSet.cmdCode = 0; printf("In Command Process LOIF => SET LO =>\n");
  }

  else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_501))      // SET_IF
  {      
     ifloSet.cmdCode = 1;  printf("In Command Process LOIF => SET IF =>\n");   
  }

  else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_502)) // NULL
  {

    ifloSet.cmdCode = 2;
    printf("In Command Process LOIF => NULL =>\n");
  }

  else if(! strcmp(Wrpr_Cmd->cmdelem.cmd_name,Cmd_503))           // domon
  {

   resp_msg_cntr=0 ;

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
  for (j = 0 ; j < Wrpr_Cmd->cmd_data.numpkt;j ++)
  { 
       for (i = 0 ; i < TSET; i++ )
      {
        
          if(! strcasecmp(Wrpr_Cmd->cmd_data.prmname[j], fevar[i]))
        { 
          
        switch(i)
        {
             case LO1 :               printf("In SWITCh Case LO1 ======== =>\n");
                                     ifloSet.lo[0] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                      printf("In SWITCh Case LO1 ======== =>%d\n",ifloSet.lo[0]);
                                     break;

             case LO2 :             printf("In SWITCh Case LO2 ======== =>\n");
                                    ifloSet.lo[1] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]); 
                                    printf("In SWITCh Case LO2 ======== =>%d\n",ifloSet.lo[1]);
                                     break;

             case IF_BAND_CH1   :  ifloSet.if_band[0] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    
                                     break;

             case IF_BAND_CH2   :  ifloSet.if_band[1] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                     
                                     break;

             case IF_PA_CH1         :  ifloSet.if_pa[0] = (float) atof(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    
                                     break;

             case IF_PA_CH2          :  ifloSet.if_pa[1] = (float) atof(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    
                                     break;

             case IF_PG_CH1     :  ifloSet.if_pg[0] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                     
                                     break;

             case IF_PG_CH2      :  ifloSet.if_pg[1] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                                                     
                                     break;

             case IF_ALC_CH1     :  ifloSet.if_alc[0]= (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);                               
                                    
                                     break;

             case IF_ALC_CH2     :  ifloSet.if_alc[1] = (int) atoi(Wrpr_Cmd->cmd_data.prmvalue[j]);
                                    
                                     break;

            
           }
         }
          

        }  
      }
 

 ifloCtrlCmd(Wrpr_Cmd);
  return 12;
}





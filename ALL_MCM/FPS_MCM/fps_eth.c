/*   Forming intermediate response for all system commands  
     Before forming, Command Validation function should be called,
     Which will validate the command and System.
*/
#include  "define.h"
#include  "fps_eth.h"

// ****** Forming intermediate response for all system commands ****** //

int FPS_form_inter_resp(parseCMSCmd *FPS_Wrpr_Cmd, devResponse *FPS_MCM_Resp)
{
  int loop;

  printf("\nEntering FPS_form_inter_resp");

  FPS_MCM_Resp->code = FPS_cmd_validation(FPS_Wrpr_Cmd,FPS_MCM_Resp);
  FPS_MCM_Resp->event = INTERMEDIATE;
  FPS_MCM_Resp->MCM_alarm.level=1;
  switch(FPS_MCM_Resp->code)
  {
    case 10 :  strcpy(FPS_MCM_Resp->message,"Command received successfully");
               printf("\nCommand received successfully");
               break;
    case 12 :  strcpy(FPS_MCM_Resp->message,"Command Syntax Error");
               printf("\nCommand Syntax Error");
               break;
    case 13 :  strcpy(FPS_MCM_Resp->message,"Command Incomplete");
               printf("\nCommand Incomplete");
               break;
    case 14 :  strcpy(FPS_MCM_Resp->message,"Command Invalid");
               printf("\nCommand Invalid");
               break;
    case 100:  strcpy(FPS_MCM_Resp->message,"Error Unknown");
               printf("\nError Unknown");
               break;
    default :  break;
  };

 // ******** Write in to Tx_BasicFlds *********** //
if ( FPS_MCM_Resp->code != 10 )
  FPS_MCM_Resp->event = FINAL;

  FPS_MCM_Resp->resp_elem.seq = FPS_Wrpr_Cmd->cmdelem.seq;
  FPS_MCM_Resp->resp_elem.version = FPS_Wrpr_Cmd->cmdelem.version;
  strcpy(FPS_MCM_Resp->resp_elem.subsysid,FPS_Wrpr_Cmd->cmdelem.subsysid);
  strcpy(FPS_MCM_Resp->resp_elem.cmd_id,FPS_Wrpr_Cmd->cmdelem.cmd_id);
  strcpy(FPS_MCM_Resp->resp_elem.cmd_name,FPS_Wrpr_Cmd->cmdelem.cmd_name);
  strcpy(FPS_MCM_Resp->resp_elem.timestamp,FPS_Wrpr_Cmd->cmdelem.timestamp);
  FPS_MCM_Resp->resp_elem.priority = FPS_Wrpr_Cmd->cmdelem.priority;
  FPS_MCM_Resp->resp_elem.timeout = FPS_Wrpr_Cmd->cmdelem.timeout;

  // ******** Write in to Tx_DataPkt *********** //
  FPS_MCM_Resp->resp_data.numpkt = 0;
  for(loop=0; loop<MAXDATA; loop++)
   {
     strcpy(FPS_MCM_Resp->resp_data.prmname[loop],"\0");
     strcpy(FPS_MCM_Resp->resp_data.prmvalue[loop],"\0");
   }
  // ******** Write in to Tx_alarm *********** //
  FPS_MCM_Resp->MCM_alarm.id = 0;
  FPS_MCM_Resp->MCM_alarm.level = 0;
  strcpy(FPS_MCM_Resp->MCM_alarm.description,"\0");

  return FPS_MCM_Resp->code;
}



// ********* This function will validate command against various checks. ********* //
// **** Syntax error, Invalid, Incomplete command, Other error will be checked **** //

int FPS_cmd_validation(parseCMSCmd *FPS_Wrpr_Cmd, devResponse *FPS_MCM_Resp)
 {
   int cmpl=0, i, j;
  /* char tmpstr[256];

    strcpy(tmpstr,FPS_Wrpr_Cmd->cmdelem.cmd_name);
    strcat(tmpstr, FPS_Wrpr_Cmd->cmdelem.subsysid);
    printf("\n #### Command received: %s \n", tmpstr);

    strcpy(tmpstr, FPS_Wrpr_Cmd->cmdelem.timestamp); printf("time: %s \n", tmpstr);
    strcpy(tmpstr, FPS_Wrpr_Cmd->cmdelem.cmd_id);  printf("cmdId: %s \n", tmpstr);

  //  printf("NumPkt %d \n", Wrpr_Cmd->cmd_data.numpkt);

    for (i=0;i< FPS_Wrpr_Cmd->cmd_data.numpkt;i++)
    {
     strcpy(tmpstr,FPS_Wrpr_Cmd->cmd_data.prmname[i]); //printf("Name %s ", tmpstr);
     strcpy(tmpstr,FPS_Wrpr_Cmd->cmd_data.prmvalue[i]); //printf("value %s \n", tmpstr);
    } */



   if(!strcmp(FPS_Wrpr_Cmd->cmdelem.subsysid,SYSTEM_4))
       {
          //printf("\nSubsystem validated");
         
         if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_400))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_401))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_402))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_403))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_404))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_405))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_406))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_407))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_408))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_409))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_410))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_411))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_412))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_413))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_414))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_415))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_416))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_417))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_418))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_419))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_420))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_421))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_422))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_423))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_424))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_425))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_426))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_427))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_428))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_429))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_430))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_431))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_432))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_433))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_434))
          {
           return 10;
          }
         else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_435))
          {
           return 10;
          }
       /*  else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_436))
          {
           return 10;

           for(j=0; j<FPS_Wrpr_Cmd->cmd_data.numpkt; j++) // Looking for Jth parameter //
            {
             for(i=0; i<SMP_rawsize; i++) // comparing Jth against Ith parameter //
              {
               if(!strcmp(FPS_Wrpr_Cmd->cmd_data.prmname[j],SenMon_Para[i]))
                {
                 cmpl++;
                 break;
                }
              }
            }// Checking of all parameters over here //

            if(cmpl == FPS_Wrpr_Cmd->cmd_data.numpkt)
             return 10;
            else
             return 13;   // Incase of Incomplete command //
          }     */
          else if(!strcmp(FPS_Wrpr_Cmd->cmdelem.cmd_name,Cmd_437))
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
   return 14;        // Incase of Invalid command //
   }

 }

/* Final Response function will in turn call FPS_command process function */

// ******** Forming final response ******** //

void FPS_form_final_resp(parseCMSCmd *FPS_Wrpr_Cmd, devResponse *FPS_MCM_Resp)
{
  printf("\nEntering form_final_resp");
  FPS_resp_msg_cntr = 0 ;
  FPS_resp_code = 10;
  FPS_MCM_Resp->code = FPS_resp_code;
  FPS_MCM_Resp->event = command_process_fps(FPS_Wrpr_Cmd,FPS_MCM_Resp);
  FPS_MCM_Resp->resp_data.numpkt = FPS_resp_msg_cntr;

 printf("\n ===> form_final_resp  :: EVENT %d CODE %d NUMPKT %d \n", FPS_MCM_Resp->event, FPS_MCM_Resp->code, FPS_MCM_Resp->resp_data.numpkt);
}




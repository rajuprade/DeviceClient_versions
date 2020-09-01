/* A simple server in the internet domain using TCP
   demonstrating how to send structure over sockets ******/
#include"server1.h"
#include <time.h>
int readable_timeo(int fd, int sec)
{
     fd_set rset;
     struct timeval tv;

      FD_ZERO(&rset);
      FD_SET(fd, &rset);

      tv.tv_sec = sec;
     tv.tv_usec = 0;

     return (select(fd + 1, &rset, NULL, NULL, &tv));
        /* > 0 if descriptor is readable */
}

/***** TIMESTAMP function ****/

char* tp(char *tstamp)
{
 char stamp1[26], stamp2[21];
 time_t t1;
   time(&t1);
   ctime_r(&t1, stamp1);
 //  fprintf(stderr,"%s\n",stamp1);
   stamp2[0] = stamp1[8];
   stamp2[1] = stamp1[9];
   stamp2[2] = '-';
   stamp2[3] = stamp1[4]; 
   stamp2[4] = stamp1[5]; 
   stamp2[5] = stamp1[6];
   stamp2[6] = '-'; 
   stamp2[7] = stamp1[20];
   stamp2[8] = stamp1[21];
   stamp2[9] = stamp1[22];
   stamp2[10] = stamp1[23];
   stamp2[11]= ' ';
   stamp2[12] = stamp1[11];
   stamp2[13] = stamp1[12];
   stamp2[14] = stamp1[13];
   stamp2[15] = stamp1[14];
   stamp2[16] = stamp1[15];
   stamp2[17] = stamp1[16];
   stamp2[18] = stamp1[17];
   stamp2[19] = stamp1[18];
   stamp2[20] = '\0'; 
   strcpy(tstamp,stamp2);
 // fprintf(stderr,"%s\n",stamp2);
  return (tstamp);
}
/******TIMESTAMP function ****/


/******* front_end *******/
void front_end(parseCMSCmd *c1)
{
     struct tm *t;  long  devtime;
     int i;
     char FB1[20],FB2[20];
      devtime = time(NULL);
                    t= localtime(&devtime);
                  //  strcpy(drptr->data.prmname[0],"time_of_day");
                   // sprintf(c1->cmdelem.timestamp,"%.2d:%.2d:%.2d",&t->tm_hour,&t->tm_min,&t->tm_sec);
     printf("Enter the Operation Code : 0 for Null, 1 for SETRFSYS, 3 for Domon,4 for URSETRFSYS\n");
     scanf("%d",&i);
        char *tstamp;
       tstamp = malloc(25);
        tstamp = tp(tstamp);
         strcpy(c1->cmdelem.timestamp,tstamp);
        strcpy(c1->cmdelem.subsysid,"fecb");

           c1->cmdelem.seq= 11;
            c1->cmdelem.version= 11;
           c1->cmdelem.timeout= 10;
            c1->cmdelem.priority= 1;

            switch(i)
             {
                case 0: strcpy(c1->cmdelem.cmd_id,"201");
                        strcpy(c1->cmdelem.cmd_name,"null");
                        break;

                case 1 : strcpy(c1->cmdelem.cmd_id,"2");
                         strcpy(c1->cmdelem.cmd_name,"setrfsys");
                         c1->cmd_data.numpkt=12;

                       strcpy(c1->cmd_data.prmname[0],"band_select_ch1");
                       strcpy(c1->cmd_data.prmname[1],"band_select_ch2");
                       strcpy(c1->cmd_data.prmname[2],"rf_swap");
                       strcpy(c1->cmd_data.prmname[3],"sol_atten_ch1");
                       strcpy(c1->cmd_data.prmname[4],"sol_atten_ch2");
                       strcpy(c1->cmd_data.prmname[5],"fe_ngcal");
                       strcpy(c1->cmd_data.prmname[6],"fe_walsh_sw");
                       strcpy(c1->cmd_data.prmname[7],"fe_walsh_grp");
                       strcpy(c1->cmd_data.prmname[8],"fe_ngcycle");
                       strcpy(c1->cmd_data.prmname[9],"rfcm_sw");
                       strcpy(c1->cmd_data.prmname[10],"ngset");
                       strcpy(c1->cmd_data.prmname[11],"fe_walsh_freq");
                       printf("Enter the Channel 1 & Channel 2 Frequency Band\n");
                       scanf("%s %s",FB1,FB2);
                       
                       strcpy(c1->cmd_data.prmvalue[0],FB1);
                       strcpy(c1->cmd_data.prmvalue[1],FB2);
                       strcpy(c1->cmd_data.prmvalue[2],"0");
                       strcpy(c1->cmd_data.prmvalue[3],"0");
                       strcpy(c1->cmd_data.prmvalue[4],"0");
                       strcpy(c1->cmd_data.prmvalue[5],"0");
                       strcpy(c1->cmd_data.prmvalue[6],"0");
                       strcpy(c1->cmd_data.prmvalue[7],"0");  
                       strcpy(c1->cmd_data.prmvalue[8],"0");
                       strcpy(c1->cmd_data.prmvalue[9],"0");
                       strcpy(c1->cmd_data.prmvalue[10],"0"); 
                       strcpy(c1->cmd_data.prmvalue[11],"0");  
                        break;

                  case 3:   strcpy(c1->cmdelem.cmd_id,"203");
                            strcpy(c1->cmdelem.cmd_name,"domon"); 
                            break;
                  case 4: strcpy(c1->cmdelem.cmd_id,"15");
                         strcpy(c1->cmdelem.cmd_name,"seturfsys");
                         c1->cmd_data.numpkt=12;

                       strcpy(c1->cmd_data.prmname[0],"band_select_ch1");
                       strcpy(c1->cmd_data.prmname[1],"band_select_ch2");
                       strcpy(c1->cmd_data.prmname[2],"rf_swap");
                       strcpy(c1->cmd_data.prmname[3],"sol_atten_ch1");
                       strcpy(c1->cmd_data.prmname[4],"sol_atten_ch2");
                       strcpy(c1->cmd_data.prmname[5],"fe_ngcal");
                       strcpy(c1->cmd_data.prmname[6],"fe_walsh_sw");
                       strcpy(c1->cmd_data.prmname[7],"fe_walsh_grp");
                       strcpy(c1->cmd_data.prmname[8],"fe_ngcycle");
                       strcpy(c1->cmd_data.prmname[9],"rfcm_sw");
                       strcpy(c1->cmd_data.prmname[10],"ngset");
                       strcpy(c1->cmd_data.prmname[11],"fe_walsh_freq");
                       printf("Enter the Channel 1 & Channel 2 Frequency Band\n");
                       scanf("%s %s",FB1,FB2);
                       
                       strcpy(c1->cmd_data.prmvalue[0],FB1);
                       strcpy(c1->cmd_data.prmvalue[1],FB2);
                       strcpy(c1->cmd_data.prmvalue[2],"0");
                       strcpy(c1->cmd_data.prmvalue[3],"0");
                       strcpy(c1->cmd_data.prmvalue[4],"0");
                       strcpy(c1->cmd_data.prmvalue[5],"0");
                       strcpy(c1->cmd_data.prmvalue[6],"1");
                       strcpy(c1->cmd_data.prmvalue[7],"1");  
                       strcpy(c1->cmd_data.prmvalue[8],"1");
                       strcpy(c1->cmd_data.prmvalue[9],"1");
                       strcpy(c1->cmd_data.prmvalue[10],"1");
                       strcpy(c1->cmd_data.prmvalue[11],"1");  
                        break;

                  }
        
                
                     
        
     
}

/************** front_end *****/



/**************self_test **********/

/* exec_cmd() *****/

void exec_cmd(int n,parseCMSCmd *c1)
{

   switch(n)
   { 
     
     case 1:front_end(c1); break;
      default: break;
   }
}

void printBasicFld(BasicFlds *bptr) {
    fprintf(stderr, "\n\t seq [%ld]  Subsystemid [%s]  \n\t ID [%s] CMDName [%s] \n\t TimeStamp [%s] \n",
       bptr->seq, bptr->subsysid, bptr->cmd_id, bptr->cmd_name, bptr->timestamp);

}

void printAlarmPkt(RespAlarm *alrm)
{
   fprintf(stderr, " Id %d %d %s \n", alrm->id, alrm->level, alrm->description);
}

void printDataPkt(DataPkt *dpkt) {
   int i;
    fprintf(stderr, "Number of Arg [%d] \n", dpkt->numpkt);
  
    for(i=0;i<dpkt->numpkt;i++) {
         fprintf(stderr, "%d name %s Value %s \n", dpkt->numpkt, dpkt->prmname[i], dpkt->prmvalue[i]);
    }
   
}

void printDevResp(devResponse *drptr)
{
         printBasicFld(&drptr->resp_elem);
         printDataPkt(&drptr->resp_data);
         printAlarmPkt(&drptr->MCM_alarm);
         fprintf(stderr, " Code %d  Event %d Message %s \n", drptr->code, drptr->event, drptr->message);

}
void printCmdPkt(parseCMSCmd *cmdptr)
{
         printBasicFld(&cmdptr->cmdelem);
         printDataPkt(&cmdptr->cmd_data);
}

/*********exec_cmd() ******/

/* Write-Read ****/
void write_read(int newsockfd,parseCMSCmd *c1, devResponse *r1)
{
      
         write(newsockfd,c1,sizeof(parseCMSCmd));
         printCmdPkt(c1);
         sleep(2);
         if (readable_timeo(newsockfd, 5) == 0)
          {
               fprintf(stderr, "socket timeout\n");
               close(newsockfd);
         }
       else 
          {
             read(newsockfd,r1,sizeof(devResponse));
             printDevResp(r1);
             sleep(1);
             read(newsockfd,r1,sizeof(devResponse));
             printDevResp(r1);

          }
          sleep(1);
}

/************************write_read*****************/

int main(int argc, char *argv[])
{
       int n;
       parseCMSCmd *c1;
       c1 = malloc(sizeof(parseCMSCmd));
       devResponse *r1;
       r1 = malloc(sizeof(devResponse));

      int sockfd, newsockfd,yes=1,i=0;
      socklen_t clilen;
      struct sockaddr_in serv_addr, cli_addr;
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
       fprintf(stderr,"ERROR opening socket\n");
    if(setsockopt(sockfd, SOL_SOCKET , SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
          error(" Error\n");
    } 
     bzero((char *) &serv_addr, sizeof(serv_addr));
    // portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORT);

     if (bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
              { 
               fprintf(stderr,"ERROR binding socket\n");
              }

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     fprintf(stderr,"############# SERVER WANTING FOR CLIENT CONNECTION #####\n");
      newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
  
    for(;;)
      {
         n =1;
        exec_cmd(n,c1);
     /*   fprintf(stderr,"############# SERVER WANTING FOR CLIENT CONNECTION #####\n");
          newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen); */
        printf("=====> ###########sizeof(parseCMSCmd) is %d\n",parseCMSCmdSize);
        printf("=====> sizeofsizeof(devResponse) is %d\n",devResponseSize);
 
        write_read(newsockfd,c1,r1); 
      //  close(newsockfd);
      }                          
                                     
      return 0; 
}

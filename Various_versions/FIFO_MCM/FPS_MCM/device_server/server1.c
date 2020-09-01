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


/******* fps *******/
void fps(parseCMSCmd *c1)
{
     struct tm *t;  long  devtime;
     int i;
     char FB1[20],FB2[20];
      devtime = time(NULL);
                    t= localtime(&devtime);
                  //  strcpy(drptr->data.prmname[0],"time_of_day");
                   // sprintf(c1->cmdelem.timestamp,"%.2d:%.2d:%.2d",&t->tm_hour,&t->tm_min,&t->tm_sec);
     printf("Enter the Operation Code :\n");
     printf("0 : null\n");
     printf("1 : domon\n");
     printf("2 : set turning point\n");
     printf("3 : set ramp down count\n");
     printf("4 : set lower RPM Limit\n");
     printf("5 : set brake down difference\n");
     printf("6 : set ramp up count\n");
     printf("7 : set stop time count\n");
     printf("8 : set max angle \n");
     printf("9 : set min angle\n");
     printf("10: set max pwm count \n");
     printf("11: read turnning point\n");
     printf("12: read ramp down count\n");
     printf("13: read lower rpm limit \n");
     printf("14: read brake down diff\n");
     printf("15: read ramp up count\n");
     printf("16: read stop time count \n");
     printf("17: read Max angle\n");
     printf("18: read Min angle\n");
     printf("19: read Max pwm count\n");
     printf("20: read version \n");
     printf("21: read UA0 angle \n");
     printf("22: run to calibrate \n");
     printf("23: free run  \n");
     printf("24: UA0CALIB\n");
     printf("25: run to C preset\n");
     printf("26: run to D preset\n");
     printf("27: run C fine tune \n");
     printf("28: run D fine tune\n");
     printf("29: reboot\n");
     printf("30: stop\n");
     printf("31: load fps position\n");
     printf("32: Move FPS to 150\n");
     printf("33: Move FPS to 375\n");
     printf("34: Move FPS to 700\n");
     printf("35: Move FPS to 1420\n");
     printf("36: fps init\n");
     printf("37: Reset\n");
     printf("38: Settime\n");
     printf("Enter the Operation Code :");

     //  scanf("%d",&i);
       char *tstamp;
       tstamp = malloc(25);
        tstamp = tp(tstamp);
         strcpy(c1->cmdelem.timestamp,tstamp);
        strcpy(c1->cmdelem.subsysid,"fps");

           c1->cmdelem.seq= 11;
            c1->cmdelem.version= 11;
           c1->cmdelem.timeout= 10;
            c1->cmdelem.priority= 1;
            i=0;
            switch(i)
             {
                case 0: strcpy(c1->cmdelem.cmd_id,"400");
                        strcpy(c1->cmdelem.cmd_name,"fpsnull");
                        break;

                case 1 : strcpy(c1->cmdelem.cmd_id,"401");
                         strcpy(c1->cmdelem.cmd_name,"domon");
                         break;

                case 2:  strcpy(c1->cmdelem.cmd_id,"402");
                         strcpy(c1->cmdelem.cmd_name,"set_tng_pnt"); 
                         break;

                case 3:  strcpy(c1->cmdelem.cmd_id,"403");
                         strcpy(c1->cmdelem.cmd_name,"set_rmp_dncnt"); 
                         break;

                case 4:  strcpy(c1->cmdelem.cmd_id,"404");
                         strcpy(c1->cmdelem.cmd_name,"set_lrpm_lmt");
                         break;

                case 5 : strcpy(c1->cmdelem.cmd_id,"405");
                         strcpy(c1->cmdelem.cmd_name,"set_brkcnt_diff ");
                         break;

                case 6:  strcpy(c1->cmdelem.cmd_id,"406");
                         strcpy(c1->cmdelem.cmd_name,"set_rpm_upcnt"); 
                         break;

                case 7:  strcpy(c1->cmdelem.cmd_id,"407");
                         strcpy(c1->cmdelem.cmd_name,"set_stop_cnt"); 
                         break;

                case 8:  strcpy(c1->cmdelem.cmd_id,"408");
                         strcpy(c1->cmdelem.cmd_name,"set_max_ang");
                         break;

                case 9 : strcpy(c1->cmdelem.cmd_id,"409");
                         strcpy(c1->cmdelem.cmd_name,"set_min_ang");
                         break;

                case 10:  strcpy(c1->cmdelem.cmd_id,"410");
                         strcpy(c1->cmdelem.cmd_name,"set_max_pwm"); 
                         break;

                case 11:  strcpy(c1->cmdelem.cmd_id,"411");
                         strcpy(c1->cmdelem.cmd_name,"rd_tng_pnt"); 
                         break;

                case 12:  strcpy(c1->cmdelem.cmd_id,"412");
                         strcpy(c1->cmdelem.cmd_name,"rd_rmp_dncnt");
                         break;

                case 13 : strcpy(c1->cmdelem.cmd_id,"413");
                         strcpy(c1->cmdelem.cmd_name,"rd_lrpm_lmt");
                         break;

                case 14:  strcpy(c1->cmdelem.cmd_id,"414");
                         strcpy(c1->cmdelem.cmd_name,"rd_brkcnt_diff"); 
                         break;

                case 15: strcpy(c1->cmdelem.cmd_id,"415");
                         strcpy(c1->cmdelem.cmd_name,"rd_rmp_upcnt"); 
                         break;

                case 16: strcpy(c1->cmdelem.cmd_id,"416");
                         strcpy(c1->cmdelem.cmd_name,"rd_stop_cnt");
                         break;

                case 17: strcpy(c1->cmdelem.cmd_id,"417");
                         strcpy(c1->cmdelem.cmd_name,"rd_max_ang");
                         break;

                case 18:  strcpy(c1->cmdelem.cmd_id,"418");
                         strcpy(c1->cmdelem.cmd_name,"rd_min_ang"); 
                         break;
                case 19:  strcpy(c1->cmdelem.cmd_id,"419");
                         strcpy(c1->cmdelem.cmd_name,"rd_max_pwm"); 
                         break;

                case 20:  strcpy(c1->cmdelem.cmd_id,"420");
                         strcpy(c1->cmdelem.cmd_name,"rd_version");
                         break;

                case 21 : strcpy(c1->cmdelem.cmd_id,"421");
                         strcpy(c1->cmdelem.cmd_name,"rd_ua0_ang");
                         break;

                case 22:  strcpy(c1->cmdelem.cmd_id,"422");
                         strcpy(c1->cmdelem.cmd_name,"calibrate"); 
                         break;

                case 23:  strcpy(c1->cmdelem.cmd_id,"423");
                         strcpy(c1->cmdelem.cmd_name,"runfree"); 
                         break;

                case 24:  strcpy(c1->cmdelem.cmd_id,"424");
                         strcpy(c1->cmdelem.cmd_name,"ua0calib");
                         break;

                case 25: strcpy(c1->cmdelem.cmd_id,"425");
                         strcpy(c1->cmdelem.cmd_name,"runcpreset");
                         break;

                case 26: strcpy(c1->cmdelem.cmd_id,"426");
                         strcpy(c1->cmdelem.cmd_name,"rundpreset"); 
                         break;

                case 27: strcpy(c1->cmdelem.cmd_id,"427");
                         strcpy(c1->cmdelem.cmd_name,"finectune"); 
                         break;

                case 28: strcpy(c1->cmdelem.cmd_id,"428");
                         strcpy(c1->cmdelem.cmd_name,"finedtune"); 
                         break;

                case 29: strcpy(c1->cmdelem.cmd_id,"429");
                         strcpy(c1->cmdelem.cmd_name,"fpsboot"); 
                         break;

                case 30: strcpy(c1->cmdelem.cmd_id,"430");
                         strcpy(c1->cmdelem.cmd_name,"fpsstop"); 
                         break;

                case 31: strcpy(c1->cmdelem.cmd_id,"431");
                         strcpy(c1->cmdelem.cmd_name,"ldfpspos"); 
                         break;

                case 32: strcpy(c1->cmdelem.cmd_id,"432");
                         strcpy(c1->cmdelem.cmd_name,"mvfps150"); 
                         break;

                case 33: strcpy(c1->cmdelem.cmd_id,"433");
                         strcpy(c1->cmdelem.cmd_name,"mvfps375"); 
                         break;

                case 34: strcpy(c1->cmdelem.cmd_id,"434");
                         strcpy(c1->cmdelem.cmd_name,"mvfps700"); 
                         break;

                case 35: strcpy(c1->cmdelem.cmd_id,"435");
                         strcpy(c1->cmdelem.cmd_name,"mvfps1420"); 
                         break;

                case 36: strcpy(c1->cmdelem.cmd_id,"436");
                         strcpy(c1->cmdelem.cmd_name,"initfps"); 
                         break;

                case 37: strcpy(c1->cmdelem.cmd_id,"437");
                         strcpy(c1->cmdelem.cmd_name,"reset"); 
                         break;

                case 38: strcpy(c1->cmdelem.cmd_id,"438");
                         strcpy(c1->cmdelem.cmd_name,"settime"); 
                         break;


                  }
        
                
                     
        
     
}

/************** fps *****/



/**************self_test **********/

/* exec_cmd() *****/

void exec_cmd(int n,parseCMSCmd *c1)
{

   switch(n)
   { 
     
     case 1:fps(c1); break;
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

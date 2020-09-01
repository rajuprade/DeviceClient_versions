#include <stdio.h>
#include <unistd.h>    /* Symbolic Constants */
#include <fcntl.h>
#include <sys/types.h> /* Primitive System Data Types */
#include <errno.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <setjmp.h>
#include <signal.h>
#include <pthread.h>
#include <poll.h>

#include "define.h"
#include "wrapperCom.h"
#include "fecb_eth.h"

int reqFd ;

jmp_buf set_pt ;  /* typedef int */


 static void signal_handler( int ) ;

     static void signal_handler(int sig){
   
             extern unsigned int time_interval;
             switch (sig)     {
               case SIGHUP  :  
               case SIGINT  :  
               case SIGSEGV :   
               case SIGPIPE :
                               longjmp(set_pt,sig);
               default:       fprintf(stderr, " signal_handler() : Signal received %d \n",sig);
             }
     }

void main(int argc, char *argv[])
{
     
        fecbfifo = FECB_FIFO; 
       
     
         
    /* if (mkfifo (fecbfifo, O_CREAT|0777) == -1)
       {
         printf("Problem in CREATING FECB FIFO \n"); 
       } */
      unlink("/tmp/fecmd_fifo");
    if (mkfifo (fecbfifo, 0666) == -1)
       {
         printf("Problem in CREATING FECB FIFO \n"); 
         printf("Unable to create a fifo; errno=%d\n",errno);
       } 
       else
         {
           printf("Successfully in CREATING FECB FIFO \n"); 
         }
         
    // mkfifo(fecbfifo,S_IWUSR | S_IRUSR |S_IRGRP | S_IROTH); 
     // sleep(5);
     /* fifo_fd = open(fecbfifo,O_WONLY|O_NONBLOCK); 
      if(fifo_fd == -1)
      {
         printf("Problem in Opening FECB FIFO\n");printf("Unable to Open FECB fifo; errno=%d\n",errno); 
      }
      else
      {
        printf("FECB FIFO ID in Command_process() ====> %d\n",fifo_fd);
      }*/ 
    extern int reqFd ;
    parseCMSCmd *cmdptr;
    devResponse *respptr;
    int rc;
    static int return_from_longjmp_to_setjmp ;
    int catchRevent = 0;

  fprintf(stderr, " BasicFlds %ld \n", sizeof(BasicFlds));
  fprintf(stderr, " DataPkt %ld \n", sizeof(DataPkt));
  fprintf(stderr, " RespAlarm %ld \n", sizeof(RespAlarm));
  fprintf(stderr, " devResponse %ld \n", sizeof(devResponse));
  fprintf(stderr, " parseCMSCmd %ld \n", sizeof(parseCMSCmd));

  /* To catch the consumCMS termination so that  DeviceComm can be reset */
           (void) signal(SIGHUP,SIG_IGN); (void) signal(SIGINT,signal_handler);
           (void) signal(SIGQUIT,signal_handler); (void) signal(SIGILL,signal_handler);
           (void) signal(SIGTRAP,signal_handler); (void) signal(SIGIOT,signal_handler);
           (void) signal(SIGFPE,signal_handler);  (void) signal(SIGBUS,signal_handler);
           (void) signal(SIGSEGV,signal_handler); (void) signal(SIGPIPE,signal_handler);
           (void) signal(SIGALRM,signal_handler); (void) signal(SIGTERM,signal_handler);
           (void) signal(SIGUSR1,signal_handler); (void) signal(SIGUSR2,signal_handler);

    if ( (cmdptr = (parseCMSCmd *) calloc( parseCMSCmdSize, 1)) == NULL ) {
       fprintf(stderr, " ### ERROR : deviceClient() : Memory for cmdptr could not allocate \n");
       exit(1);
    }        

    if ( (respptr = (devResponse *) calloc( devResponseSize, 1)) == NULL ) {
       fprintf(stderr, " ### ERROR : deviceClient() : Memory for cmdptr could not allocate \n");
       exit(1);
    }        

  do {

    if( (reqFd = initClientConn(reqPORT)) == ERROR ) {
        fprintf(stderr, "### ERROR : deviceClient() reqFd %d: could not connect to RequestPort %d \n", reqFd, reqPORT);
    }else {
        fprintf(stderr, "deviceClient() : DO LOOP Successfully connected to RequestPort %d #####REQUEST=>FD %d\n", reqPORT,reqFd);
         // recvCmdPkt(&reqFd,cmdptr);
         
            //  printCmdPkt(cmdptr); 
              //formDeviceResp(cmdptr, respptr);
          
    }
    sleep(1);
  } while (reqFd < 0 );

  /* setjmp return 0 if success */
   if((return_from_longjmp_to_setjmp = setjmp(set_pt)) != 0 )
   {
          rc = return_from_longjmp_to_setjmp ;
          fprintf(stderr, " \n#### singal_handler() :: Catched  signal %d \n",rc);
    
        do {
          if( (reqFd = initClientConn(reqPORT)) == ERROR ) {
              fprintf(stderr, "### ERROR : deviceClient() reqFd %d: could not connect to RequestPort %d \n", reqFd,reqPORT);
          }else {
              fprintf(stderr, "deviceClient() : LONG JUMP LOOP Successfully connected to RequestPort %d \n", reqPORT);
             }
          sleep(1);
        } while (reqFd < 0 );
 
  }

  while(1) 
  {
           // Read event
           bzero(cmdptr,parseCMSCmdSize); 
           if ( (catchRevent = recvCmdPkt(&reqFd,cmdptr)) > 0 )  // == 0 was there but now checking
           {
              printCmdPkt(cmdptr);
               fifo_fd = open(fecbfifo,O_WRONLY|O_NONBLOCK); 
               if(fifo_fd == -1)
               {
                   printf("Problem in Opening FECB FIFO\n");printf("Unable to Open FECB fifo; errno=%d\n",errno); 
                }
                 else
                 {
                    printf("FECB FIFO ID in Command_process() ====> %d\n",fifo_fd);
                 }
              form_inter_resp(cmdptr,respptr);
              sendRespPkt(&reqFd,respptr);
              FE_form_final_resp(cmdptr,respptr);
                 respptr= &dev_Resp;
               sendRespPkt(&reqFd,respptr);
               printDevResp(respptr);
               close(fifo_fd);
              //formDeviceResp(cmdptr, respptr);
           }
  
              usleep(500000);
  }
}

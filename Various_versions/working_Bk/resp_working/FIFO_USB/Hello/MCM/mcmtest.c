/******************************************************************
 * mcmtest.c @Author Raj Uprade
 * C test program for  MCM Communication
 * requires USB to RS485 converter cable by FTDI
 * As per ABR lab requirement implemented the SET L0 commands which takes as input lo1 lo2 and 
    convert respective frequencies into 32 or 64 bit digital mask argument dt.24.2.2012 

******************************************************************/
// C program to implement one side of FIFO 
// This side reads first, then reads 
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <errno.h>
#include <stdlib.h>
#include <sys/io.h> 
#include <time.h>
#include <pthread.h>
#include<time.h>
#include<fcntl.h>
#include "/home/raj/Desktop/FIFO_USB/Hello/ftd2xx.h" 

int resp_code;


typedef struct
 {
    int Length;
    char Msg[200];
 }  serialBuff ;

serialBuff wbuff,rbuff;

//void add_cksum(char buff[]);
void write_read(char *buff);

unsigned char buff[100];

#define BUF_SIZE  0x10
#define DEVICE_ID "FTSERRS1"



char *respfps;
int resp_fd;   
#define RESP_FIFO "/tmp/resp_fps"


char *respfecb;
int resp_fecbfd;   
#define RESP_FECB "/tmp/resp_fecb"

/*--------------Add Check Sum function---------------------------------*/
/* void add_cksum(char buff[])
{
 int length, i;
 char CS=0;

 length = buff[1] + (buff[2]<<8);
 for(i=1; i<length; i++) CS += *(buff+i);
 CS = ~CS+1;
// length = length+1;
 buff[length] = CS;
   #ifdef DEBUG 
   printf("I am inside Cheksum function :\n");
   fprintf(stderr, "I m in Add_chksum%.2x\t\n", (unsigned char)(buff[length])); 
  #endif
}*/
/*------------ Write & read function----------------------------------*/
void write_read(char *buff)
{
  
   long bytes=0;
   int length,i,j,k;
   char CS=0;
   float volt;
   time_t tim;
   time(&tim);
   unsigned char  *pcBufRead = NULL, *testBuf = NULL;
   length = buff[1] + (buff[2]<<8);

    respfps=RESP_FIFO;
    resp_fd = open(respfps,O_WRONLY|O_NONBLOCK); 
    
             if(resp_fd==-1)
               { printf("Problem in Opening RESPONSE FPS FIFO\n");printf("Unable to OPEN a RESPONSE FPS FIFO ; errno=%d\n",errno); }
              else
               { printf("Successful in Opening RESPONSE FPS FIFO\n");
               } 
     respfecb=RESP_FECB;
    resp_fecbfd = open(respfecb,O_WRONLY|O_NONBLOCK); 
    
             if(resp_fecbfd==-1)
               { printf("Problem in Opening RESPONSE FECB FIFO\n");printf("Unable to OPEN a RESPONSE FECB FIFO ; errno=%d\n",errno); }
              else
               { printf("Successful in Opening RESPONSE FECB FIFO\n");
               } 
   // buff[0]=*buf_ptr0;
    
   // printf("++++++++++++++++++++++++++++++++++++++ Mcm addr %d #######################################\n", buff[0]); 
              
    //  add_cksum(buff);  
    
                /*  for (i=0; i<(length+1); i++)
                  printf("NULL PKT in USB_RS485 ====>0x%02X\n", buff[i]); */
   
   /*  for (i=0; i<(length+1); i++)
                  printf("Command packet PKT in USB_RS485 ====>0x%02X\n", buff[i]); */
    DWORD dwBytesWritten = 0,dwBytesInQueue = 0 ;
    DWORD dwRxSize = 2;
    FT_STATUS ftStatus;
    FT_HANDLE ftHandle;
    EVENT_HANDLE eh;
  
          if((ftStatus = FT_OpenEx(DEVICE_ID, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)
                  {
			/* 
				This can fail if the ftdi_sio driver is loaded
		 		use lsmod to check this and rmmod ftdi_sio to remove
				also rmmod usbserial
		 	*/
			printf("Error FT_OpenEx(%d)\n", ftStatus);
			
		  }
                else
                   {
                      fprintf(stderr," Device opened successfully\n");
                   }   
	
                    ftStatus = FT_SetTimeouts(ftHandle, 1000, 1000);
                       if (ftStatus == FT_OK) {
                                    
                                        fprintf(stderr," Device SET timeout ok \n");
                                           }
                                            else {
                                                                  
                                                           fprintf(stderr," Device SET timeout FAiled \n");
                                                       }

            FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);  // clean the Tx and RX buffers...

                ftStatus = FT_SetRts(ftHandle);
                if (ftStatus == FT_OK) 
                       {
                         // FT_SetRts OK
                          fprintf(stderr,"FT_SetRTS(%d)\n", ftStatus);
                       }
                  else 
                      {
                                // FT_SetRts failed
                                 fprintf(stderr,"ERROR FT_SetRTS(%d)\n", ftStatus);
                       }
       
		ftStatus = FT_SetBaudRate(ftHandle, 9600); 
                if (ftStatus == FT_OK) 
                   {
                     // FT_SetBaudRate OK
                     fprintf(stderr,"FT_SetBaudRate(%d)\n", ftStatus);
                   }
                   else 
                   {
                        // FT_SetBaudRate Failed
                        fprintf(stderr,"Error FT_SetBaudRate(%d)\n", ftStatus);

                   }
       
                 // Set 8 data bits, 1 stop bit and no parity
                ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1,FT_PARITY_MARK);
                if (ftStatus == FT_OK) 
                    {
                     
                      fprintf(stderr,"SET FT_SetDataCharacteristics(%d)\n", ftStatus);
                    }
                     else 
                        {
                            
                            fprintf(stderr,"Error FT_SetDataCharacteristics(%d)\n", ftStatus);

                         }
                                   
		/* Write */
		if((ftStatus = FT_Write(ftHandle, buff, 1, &dwBytesWritten)) != FT_OK) {
			printf("Error FT_Write(%d)\n", ftStatus);
			}
                   if(buff[0]==5)
                    {usleep(0);
                    }
                    else
                   { usleep(2000); }
                     dwBytesWritten = 0; 
                    ftStatus = FT_SetBaudRate(ftHandle, 9600);

                   ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1,FT_PARITY_SPACE);
                  if (ftStatus == FT_OK)
                    {
                      // FT_SetDataCharacteristics OK
                        // fprintf(stderr,"SET FT_SetDataCharacteristics(%d)\n", ftStatus);
                    }
                    else {
                            // FT_SetDataCharacteristics Failed
                          fprintf(stderr,"Error FT_SetDataCharacteristics(%d)\n", ftStatus);
                      
                         }
                                             
                      if((ftStatus = FT_Write(ftHandle, (buff+1),length, &dwBytesWritten)) != FT_OK) {
                        printf("Error FT_Write(%d)\n", ftStatus);
                         }
                       /*  else
                          {
                                for(i=0;i<=dwBytesWritten;i++)
                                printf("We have successfully written buffer %x\n", (unsigned char)buff[i]);
                           }*/
                             
             ftStatus = FT_ClrRts(ftHandle);
               if (ftStatus == FT_OK)
                 {
                         // FT_SetRts OK
                         fprintf(stderr,"FT_ClearRTS(%d)\n", ftStatus);
                 }
                     else {
                          // FT_SetRts failed
                         fprintf(stderr,"ERROR FT_ClearRTS(%d)\n", ftStatus);
                          }
                          
                      pcBufRead = (char*)malloc(sizeof(char )*512);
                      testBuf = (char*)malloc(sizeof(char )*512);
                      memset(pcBufRead, 0x00, 512); 
                      memset(testBuf, 0x00,512); 
                      memset(buff,0x00,100);
                      length=0; 
              	             ftStatus = FT_Read(ftHandle, pcBufRead,dwRxSize, &dwBytesInQueue);
                             if (ftStatus == FT_OK) 
                                 {
                                           if(dwBytesInQueue == 0 ) 
                                               {
                                                      fprintf(stderr,"############### MCM DEVICE TIMEOUT ###########\n\n\n\n\n\n"); 
                                                                                                        
                                                } 
			        	     else
                                                { 
                                            
                                                   printf("######### RESPONSE FROM THE SYSTEM###########\n");
                                                   printf("MCM ADDRESS IS=>%d\n",(int)pcBufRead[0]);                                              
                                                   printf("PACKET LENGTH IS=>%d\n",(int)pcBufRead[1]);
                                                
                                                     rbuff.Msg[0]= pcBufRead[0];
                                                     rbuff.Msg[1]= pcBufRead[1];
                                                   #ifdef DEBUG
                                                      printf("##### RBUFF ====>MCM ADDRESS IS=>%d\n",(int)rbuff.Msg[0]);                                              
                                                      printf("$$$$$$ RBUFF PACKET LENGTH IS=>%d\n",(int)rbuff.Msg[1]);
                                                   #endif
                                               
                                                }
			            }
                 
			     else 
                                   {
                                      fprintf(stderr,"ERROR FT_TIMEOUT(%d)\n", ftStatus);
				      printf("Device TIMEOUT\n"); 
			           } 
                          

                               sprintf(testBuf,"%x",pcBufRead[1]);
  
                              dwRxSize = (DWORD)strtoul( testBuf, NULL, 16); 
                               rbuff.Length= (dwRxSize+1); 
                              /*    if ( rbuff.Length <= 0 )
                               { resp_code = 11; }
                               else { resp_code = 10; } */
   
                                 if (dwRxSize == 0 ) 
                                      goto again; 

                                  dwRxSize = dwRxSize-1;
                            

                                     dwBytesInQueue = 0;  
                               ftStatus = FT_Read(ftHandle, pcBufRead,dwRxSize, &dwBytesInQueue);
             
                                            for(j = 0; j <rbuff.Length ; j++) 
                                            {                     
                                              rbuff.Msg[j+2] = pcBufRead[j];                       
	                                    }
                                          /* write(resp_fd,rbuff.Msg,rbuff.Length);
                                           sleep(1);
                                           close(resp_fd);*/
                                     if (ftStatus == FT_OK) 
                                       {
	                        	 
                     
                                                      for(j = 0; j < dwRxSize; j++) 
                                                      {
		                                        printf("MCM RESPONSE BUFFER [%d] = 0x%02X\n", j, pcBufRead[j]);
                       
                     
		                                      } 
                                                  printf("\t\t*************************************************\n");
                                                  printf("\t\t############ MCM COMMAND SUCCESSFUL #############\n");
                                                  printf("\t\t*************************************************\n");
                                                  printf("\n");
                                                  int ret=0;
                                             
                                                  if(rbuff.Msg[0]==14)
                                                  {
                                                  ret= write(resp_fd,rbuff.Msg,rbuff.Length);
                                                  printf("############# Write Call returned %d\n @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",ret);
                                                  }
                                                  else if(rbuff.Msg[0]==5)
                                                  {
                                                  ret= write(resp_fecbfd,rbuff.Msg,rbuff.Length);
                                                  printf("############# Write Call returned %d\n @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",ret);
                                                  }
                                                  else
                                                  { printf("##### UnKNOWN MCM RESPONSE\n");}
                                                // sleep(1);
                                                 close(resp_fd);close(resp_fecbfd);
                                                  if ( rbuff.Length <= 0 )
                                                    { resp_code = 11; }
                                                     else { resp_code = 10; }                    

               
                                               // }
		                            }
			          else 
                                    {
                                        fprintf(stderr,"ERROR FT_TIMEOUT(%d)\n", ftStatus);
				          
			            }

                  
                        again:
                           free(pcBufRead);
                           free(testBuf);                             	
		        FT_Close(ftHandle);
       
}

  
int main() 
{ 
    int fd1,fps_fd,status; 
  
     
     char * myfifo = "/tmp/fecmd_fifo"; 
  
     char * myfifo1 = "/tmp/fpscmd_fifo"; 

      int ret=-1;

     ret = system("rmmod ftdi_sio");     // Check for default drivers in the system.If drivers are there.System() command removes the default drivers from the kernel.
     if(ret)
     {
       system("rmmod usbserial");
       printf("\n No need to remove Default drivers loaded\n");
     }
    else
     {
       printf("Removed the default drivers\n");
     }
 
    char str1[100], str2[100]; 
 
     fd1 = open(myfifo,O_RDONLY|O_NONBLOCK ); 
     if(fd1== -1)
      { printf("Problem in Opening FECB FIFO\n");printf("Unable to OPEN a fifo; errno=%d\n",errno); }
      else
    { printf("Successful in Opening FECB FIFO\n");} 
    fps_fd = open(myfifo1,O_RDONLY|O_NONBLOCK); 
    
      if(fps_fd==-1)
      { printf("Problem in Opening FPS FIFO\n");printf("Unable to OPEN a fifo; errno=%d\n",errno); }
     else
     { printf("Successful in Opening FPS FIFO\n");} 
    while (1) 
    { 
          
	   int i,length;
           length =0;
           memset(buff, 0x00, 100); 
           memset(str1,0x00,100);
           memset(str2,0x00,100);
           
           read(fd1,str1,100);         
           length = str1[1] + (str1[2]<<8);
          
           if((length > 7))
           {
              for (i=0; i<(length+1); i++)
              {
                printf("serial PKT in USB_RS485 ====>0x%02X\n",(unsigned char) str1[i]);
                buff[i]=str1[i];
              }
               write_read(buff);
           }
          
          
           read(fps_fd,str2,100);
            
           length = str2[1] + (str2[2]<<8);
            if((length > 7))
           {
              for (i=0; i<(length+1); i++)
              {
               printf("serial PKT in USB_RS485 for FPS SYSTEM ====>0x%02X\n",(unsigned char) str2[i]);
               buff[i]=str2[i];
              }
               write_read(buff);
           }
       
   
    } 
    return 0; 
} 

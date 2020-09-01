/******************************************************************
 * mcmtest.c
 * C test program for  MCM Communication
 * requires USB to RS485 converter cable by FTDI
* Today found out that checksum was not going out with the address and data byte.
* changed the program, then chksum was there withh the outgoing commands
* still MCM was not sending response
* Charu figure out that with serial port idle line is in high state
* But with this USb-RS 485 cable idle line is in low state so a PULL UP register is connected with Orange cable
* Now commands are going to the MCM and MCM is sending response.
* For the read part I have to write some event based mechanism to read the incoming characters. Date: 16/08/2011

*** Written the event based READ also sumtimes it's working...but sumtimes It is failing.....

**** Now READ event is working for NUll command..I am getting RESPONSE for NULL command...Dt .17.08.2011

DwRxSize plays an important role here If I make it dwRxSize < 15 , except NULL commandI get response from all commands.

I have to determine the size of dwRxSize ..so that program can be dynamic....

Manually tried reading the address and Packet bytes from the incoming packets....but it didn't work as per desired...Date 18.8.2011

Tried SetbitMode also for Large Read..but it is not working...
also tried While(ftStatus != ft_OK ) but it is also not working...Dt..23.08.11


Now I am doing two read operation and it is working fine....

just finding it bit difficult to copy data from a char pointer to a char pointer...

Now used sprintf() to copy the packet in the tetsbuf ...

and used strtoul to get the dwRxSize from the packet length...:.

Most of the commands are working fine like 
1. Null command
2. set idle
3. set d_mask 16
4.set d_mask 32   // it is giving sigmentation fault...
5.read version  
6. read mode...

Also checked Set Analog mask it is also giving response back...sumtimes segmentation fault.... Dt 26.8.11


After statically allocating memory for dmask_32 bit and set_ana_mask to 64 bytes..segmentation fault problem is solved.....

28-SEP-2011
           Implemented below mentioned commands ....tested with MCM ..all are working fine....
1.  feed_mcm_cmd
2.  feed_data_monitoring
3.  feed_control_modified
4.  fe_box_mon
5.  common_box_mon
6.  set_dmask_64b
7.  read_dmask_64b

While sending command 1st byte tell the packet length..it must be in HEX.

sumtimes set_analog_mask gives some problem.....
******************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include<string.h>
#include <sys/io.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include "/home/teleset/Desktop/Hello/ftd2xx.h"
#define BUF_SIZE  0x10
/*------------ display function-----------------------------------------*/

/* Global Variable */
  int CmdNum;
  char CmdName[30];
  unsigned char Argument[64];
  char *buf_ptr0;
   buf_ptr0 = malloc(100);
/*------------ checksum function-------------------------------------*/
char cksum(char *buff)
{
 int length, i;
 char CS=0;

 length = *(buff) + (*(buff+1)<<8);
 for(i=0; i<length; i++) CS += *(buff+i);
 return CS;
}

/*--------------Add Check Sum function---------------------------------*/
AddCksum(char *buff)
{
  int length, i;
  char cs = 0;

  length = *(buff+1) + (*(buff+2)<<8);
  for(i=0; i<(length-1); i++) cs += *(buff+i+1);
  cs = ~cs+1;
  buff[length] = cs;
}
/*------------ Write & read function----------------------------------*/
void write_read(char *buff, char *buf_ptr0)
{
   long bytes=0;
   int length,i,j;
   char CS=0;
   char  *pcBufRead = NULL, *testBuf = NULL;
   length = buff[1] + (buff[2]<<8);

/*  for(i=1; i<length; i++) CS += *(buff+i);
   CS = ~CS+1;
    //length = length+1;
   printf(" Length is %d\n", length);
   buff[length] = CS;*/
   buff[0]=*buf_ptr0;
    printf("Mcm addr %d\n", buff[0]);
      add_cksum(buff);  
    DWORD dwBytesWritten = 0,dwBytesInQueue = 0 ;
    DWORD dwRxSize = 2;
    FT_STATUS ftStatus;
    FT_HANDLE ftHandle;
    EVENT_HANDLE eh;
   
	/* Setup */
		if((ftStatus = FT_OpenEx("FTU6Z938", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK)
                  {
			/* 
				This can fail if the ftdi_sio driver is loaded
		 		use lsmod to check this and rmmod ftdi_sio to remove
				also rmmod usbserial
		 	*/
			printf("Error FT_OpenEx(%d)\n", ftStatus);
			//return ERROR;
		  }
                else
                   {
                      fprintf(stderr," Device opened successfully\n");
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
                      // FT_SetDataCharacteristics OK
                      fprintf(stderr,"SET FT_SetDataCharacteristics(%d)\n", ftStatus);
                    }
                     else 
                        {
                            // FT_SetDataCharacteristics Failed
                            fprintf(stderr,"Error FT_SetDataCharacteristics(%d)\n", ftStatus);

                         }
                                   
		/* Write */
		if((ftStatus = FT_Write(ftHandle, buff, 1, &dwBytesWritten)) != FT_OK) {
			printf("Error FT_Write(%d)\n", ftStatus);
			}
          
                        usleep(10000);
                     dwBytesWritten = 0; 
                    ftStatus = FT_SetBaudRate(ftHandle, 9600);

                   ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1,FT_PARITY_SPACE);
                  if (ftStatus == FT_OK)
                    {
                      // FT_SetDataCharacteristics OK
                         fprintf(stderr,"SET FT_SetDataCharacteristics(%d)\n", ftStatus);
                    }
                    else {
                            // FT_SetDataCharacteristics Failed
                          fprintf(stderr,"Error FT_SetDataCharacteristics(%d)\n", ftStatus);
                      
                         }
                                             
                      if((ftStatus = FT_Write(ftHandle, (buff+1),length, &dwBytesWritten)) != FT_OK) {
                        printf("Error FT_Write(%d)\n", ftStatus);
                       
                }
                             
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
                          //
	
     	usleep(10000); // Some delay is necessary because hardware can't write succesively...in a short period of time...
		
	// We Are writing the READ block of code
         pthread_mutex_init(&eh.eMutex, NULL);
	pthread_cond_init(&eh.eCondVar, NULL);

       ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0);
	if(ftStatus != FT_OK) 
          {
		printf("Failed to set flow control\n");	
	  }

	ftStatus = FT_SetEventNotification(ftHandle, FT_EVENT_RXCHAR, (PVOID)&eh);
	if(ftStatus != FT_OK) {
		printf("Failed to set events\n");
		//return 1;
	}
	
	pthread_mutex_lock(&eh.eMutex);
	pthread_cond_wait(&eh.eCondVar, &eh.eMutex);
	pthread_mutex_unlock(&eh.eMutex);
	
                      pcBufRead = (char*)malloc(sizeof(char )*512);
                      testBuf = (char*)malloc(sizeof(char )*512);
                      memset(pcBufRead, 0x00, 512); 
                      memset(testBuf, 0x00,512); 
	              	ftStatus = FT_Read(ftHandle, pcBufRead,dwRxSize, &dwBytesInQueue);
                            if (ftStatus == FT_OK) 
                           {
				        printf("FT_Read = %d\n", dwBytesInQueue);
			        	for(j = 0; j < dwBytesInQueue; j++) 
                                         {
					     printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
				         }
			   }
			 else 
                             {
                                      fprintf(stderr,"ERROR FT_TIMEOUT(%d)\n", ftStatus);
				          
			       }
//                         pcBufRead[strlen(pcBufRead)]='\0';
 
//                         fprintf(stderr, " %x \n", pcBufRead[1]);
                        sprintf(testBuf,"%x",pcBufRead[1]);
  //                       fprintf(stderr, "=====  %x %x %s \n", pcBufRead[1], testBuf[0], testBuf);
                         dwRxSize = (DWORD)strtoul( testBuf, NULL, 16); 
    //                     fprintf(stderr, " -----------\n");
      //                      printf(" DWRXSIZE is %d\n",dwRxSize);
 

                          dwRxSize = dwRxSize-1;
                            printf(" DWRXSIZE is %d\n",dwRxSize);

                             dwBytesInQueue = 0;  
                            ftStatus = FT_Read(ftHandle, pcBufRead,dwRxSize, &dwBytesInQueue);
                            if (ftStatus == FT_OK) 
                           {
				       // printf("FT_Read = %d\n", dwBytesInQueue);
			        	//for(j = 0; j < dwBytesInQueue; j++) 
			        	printf("FT_Read = %d\n",dwRxSize);
                                         for(j = 0; j < dwRxSize; j++) 
                                         {
					     printf("pcBufWrite[%d] = 0x%02X\n", j, pcBufRead[j]);
				         }
			   }
			 else 
                             {
                                      fprintf(stderr,"ERROR FT_TIMEOUT(%d)\n", ftStatus);
				          
			         }
 
                             free(pcBufRead);
               free(testBuf); 
                              //  disp_answer(pcBufRead);
                               //   free(pcBufRead);
	/* Cleanup */
		FT_Close(ftHandle);
                
        /*       free(pcBufRead);
               free(testBuf);  */
}
void MakeBuffer(Cmd)
{
  switch(Cmd) {
    case 1 : {
      char buff[]={0,8,0,0,0,0,0,0,0};
      InitBuff(buff); break;
    }
    case 2 : {
      char buff[]={0,0xa,0,0,1,0,2,0,0,0,0};
      InitBuff(buff); break;
    }
    case 3 : {
      char buff[]={0,0xa,0,0,1,0,2,0,0,1,0};
      InitBuff(buff); break;
    }
    case 4 : set_mean_mode(Cmd.Argument);break;
    case 5 : set_anl_mask(Cmd.Argument);break;
    case 6 : set_dig_mask16(Cmd.Argument);break;
    case 7 : set_dig_mask32(Cmd.Argument);break;
    case 8 : set_dig_mask64(Cmd.Argument);break;
    case 9 : {
      char buff[]={0,9,0,0,2,0,1,0,0,0};
      InitBuff(buff); break;
    }
    case 10 : {
      char buff[]={0,9,0,0,2,0,1,0,1,0};
      InitBuff(buff); break;
    }
    case 11 : {
      char buff[]={0,9,0,0,2,0,1,0,2,0};
      InitBuff(buff); break;
    }
    case 12 : {
      char buff[]={0,9,0,0,2,0,1,0,5,0};
      InitBuff(buff); break;
    }
    case 13 : {
      char buff[]={0,9,0,0,2,0,1,0,3,0};
      InitBuff(buff); break;
    }
    case 14 : {
      char buff[]={0,9,0,0,2,0,1,0,4,0};
      InitBuff(buff); break;
    }
    case 15 : {
      char buff[]={0,8,0,0,5,0,0,0,0};
      InitBuff(buff); break;
    }
    case 16 : feed_cntrl_old(Cmd.Argument);break;
    case 17 : feed_cntrl_new(Cmd.Argument);break;
    case 18 : fe_box_mon(Cmd.Argument);break;
    case 19 : {
      char buff[]={0,9,0,0,7,0,1,0,1,0};
      InitBuff(buff); break;
    }
  } 
  *buf_ptr0 = McmAddr;
  AddCksum(buf_ptr0); 
}

/******* Init buffer function ***********************
 * Make buffer sub function
 * convenient for buffer implementation            */ 
void InitBuff(char buff[])
{
  int i, length;
  length = buff[1] + (buff[2]<<8);
  for (i=0; i<length; i++) *(buf_ptr0+i) = buff[i];
}

void SetCmdArgument(void)
{
  int i;

  switch (CmdNum) {
    case (0) :
      printf("\nEnter Mcm Addr (dec):\t");
      scanf("%d", (int *)&Argument[0]);
      break;
    case (5) :
      printf("\nEnter Anl Mask (xx xx xx xx xx xx xx xx):\t");
      for (i=0;i<8;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (6) :
      printf("\nEnter Dig Mask 16 (xx xx):\t");
      for (i=0;i<2;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (7) :
      printf("\nEnter Dig Mask 32 (xx xx xx xx):\t");
      for (i=0;i<4;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (8) :
      printf("\nEnter Dig Mask 64 (xx xx xx xx xx xx xx xx):\t");
      for (i=0;i<8;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (4) :
      printf("\nEnter Avg Factor:\t");
      scanf("%d", (int *)&Argument[0]);
      break;
    case (16) :
      printf("\nEnter Feed data mask (xx):\t");
      scanf("%x", (int *)&Argument[0]);
      break;
    case (17) :
      printf("\nEnter Feed data mask(xx xx):\t");
      for (i=0;i<2;i++) scanf("%x", (int *)&Argument[i]);
      break;
    case (18) :
      printf("\nEnter Feed box number:\t");
      scanf("%d", (int *)&Argument[0]); break;
    case (21) :
     // Fofinputs(); 
     break;
    case (22) :
      //Datmoninputs();
         break;
    case (23) :
      //Filedatinputs(); 
             
  }
}

void set_mcm_addr()
{
  char McmAddr;
   McmAddr = Arg[0];
  *buf_ptr0 = McmAddr;
  printf("adr mcm set %x\n", McmAddr);
}

void set_dig_mask64(unsigned char Arg[])
{
 int i;
 char buff[]={0,0x11,0,0,1,0,9,0,4,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0};

 for(i=0; i<8; i++) buff[9+i] = Arg[i];
 InitBuff(buff);
}

void set_dig_mask32(unsigned char Arg[])
{
 int i;
 char buff[]={0,0xd,0,0,1,0,5,0,3,0xff,0xff,0xff,0xff,0};     

 for(i=0; i<4; i++) buff[9+i] = Arg[i];
 InitBuff(buff);
}

void set_dig_mask16(unsigned char Arg[])
{
  int i;
  char buff[]={0,0xb,0,0,1,0,3,0,2,0xff,0xff,0};

  for(i=0; i<2; i++) buff[9+i] = Arg[i];
  InitBuff(buff);
}

void set_anl_mask(unsigned char Arg[])
{
  int i;
  char buff[]={0,0x11,0,0,1,0,9,0,1,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0};

  for(i=0; i<8; i++) buff[9+i] = Arg[i];
  InitBuff(buff);
}

void set_mean_mode(unsigned char Arg[])
{
 char buff[]={0,0xb,0,0,1,0,3,0,0,2,0,0};

 buff[10] = Arg[0];
 InitBuff(buff);
}


void feed_cntrl_old(unsigned char Arg[])
{
  char buff[]={0,0xa,0,0,3,0,2,0,0,0,0};

  buff[8] = Arg[0]; buff[9] = Arg[0]>>8;//buff[9]=0;
  InitBuff(buff);
}

void feed_cntrl_new(unsigned char Arg[])
{
  int i;
  char buff[]={0,0xa,0,0,6,0,2,0,0,0,0};

  for(i=0; i<2; i++) buff[8+i] = Arg[i];
  InitBuff(buff);
}

void fe_box_mon(unsigned char Arg[])
{
  char buff[]={0,0xa,0,0,7,0,2,0,0,0,0};

  buff[9] = Arg[0]-1;
  InitBuff(buff);
}

int main(int argc, char *argv[])
{

 
   while(1)
    {
       printf("Type Command -1 for the list of available commands\n");
       printf("\nEnter Command:\t");
       //fflush(stdout);
       scanf("%d", &CmdNum);
       if (CmdNum == 0) 
      
     if ((CmdNum < 0) || (CmdNum > 23)) {
     printf("\n0:Set Mcm Addr\t\t1:Null Cmd\t\t2:Set Idle Mode\t\t3:Set Scan Mode\n");
    printf("4:Set Mean Mode\t\t5:Set Anl Mask\t\t6:Set Dig Mask 16b\t7:Set Dig Mask 32b\n");
    printf("8:Set Dig Mask 64b\t9:Read Anl Mask\t\t10:Read Dig Mask 16b\t11:Read Dig Mask 32\n");
    printf("12:Read Dig Mask 64b\t13:Read Version\t\t14:Read mode\t\t15:Reboot\n");
    printf("16:FE Control (old)\t17:FE Control (new)\t18:FE-Box Monitor\t19:Common Box Monitor\n");
    printf("20:Exit\t\t\t21:File of File\t\t22:Data Monitoring\t23:File Data\n");
   }
  else SetCmdArgument();

 
}
 return 1;
}



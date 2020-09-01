#include "define.h"
#include "iflo_mcm.h"
//#define DEBUG
//#define JDEBUG
int MCMCARD1 = 2, MCMCARD2 = 3, MCMCARD3=10,MCMCARD4=0,MCMCARD5=4;


void Convert(int offset, char * mask_x)
{
  int j;
  unsigned long Argument0, Argument1;
  unsigned char tab1[3], tab2[3];
 
  for (j=0;j<2;j++) {tab1[j]=mask_x[j];tab2[j]=mask_x[j+2];}     //"ffff" mask format
  tab1[2] = tab2[2] = '\0';       //strtol() converts until '\n'

 #ifdef DEBUG
  printf(" mask_x[0] %c mask_x[2] %c mask_x[1] %c mask_x[3] %c \n",mask_x[0], mask_x[2], mask_x[1], mask_x[3]);
  printf(" tab1 %s tab2 %s \n", tab1, tab2);
 #endif

  exeCmd.Argument[0+offset] =(unsigned char) strtol((const char *)tab2, NULL, 16);     //3412 order   // Hex Conversion */
  exeCmd.Argument[1+offset] = (unsigned char) strtol((const char *)tab1, NULL, 16);

 
  #ifdef DEBUG
  printf("Argument0 = %x  Argument1 = %x \n", exeCmd.Argument[0+offset],exeCmd.Argument[1+offset]);
  #endif
}

void Mask16toArg(char * mask1)
{
int j;

  for(j=0;j<sizeof(exeCmd.Argument);j++)
   exeCmd.Argument[j]=0x00;
   Convert(0, mask1);
}

void Mask32toArg(char * mask1, char * mask2)
{
int j;

  for(j=0;j<sizeof(exeCmd.Argument);j++)
   exeCmd.Argument[j]=0x00;

   Convert(0, mask1);
   Convert(2, mask2);
}

void Mask64toArg(char * mask1, char * mask2, char * mask3, char * mask4)
{
int j;

  for(j=0;j<sizeof(exeCmd.Argument);j++)
  exeCmd.Argument[j]=0x00;
  Convert(0, mask1); Convert(2, mask2);
  Convert(4, mask3); Convert(6, mask4);
}

void getbitDO(int word)
{
  int i, mask, t_bit;

   for(i=7;i >= 0;)
   {
     mask = 1<<i;
     t_bit =  word & mask;            /* and operator */
     if( t_bit == 0 ) bitD0[i] = 0;
     else bitD0[i] = 1;
     i--;
   }
#ifdef DEBUG
   printf("BIT : %d  ",word);
   for(i=0;i<7;i++) {
     printf("%d",bitD0[i]);
   }
   printf("\n");
#endif
}

/* mcm4setting() => Written for SETURFSYS command,as it have MCM 4 setting (NOISE CYCLE,WALSH PATTERN,WALSH FREQUENCY) Date: 26/07/2018 -RU */
void mcm4setting()
{
     int i , j ,b ;
     for(i=0;i<32;i++) { digWord[i]=0; } 
          
                     for (j=0; j<2; j++)
                       { 
                          
                          // ----------New logic for MCM 4, SEP 20, 2017 ---------//
                          // NG Cycle bit D0,1
                        getbitDO(fecbSet.ngCycle); for (b=0;b<2;b++) { digWord[b + (j*16)] = bitD0[b]; }
                      // NG Freq bit D2,3
                        getbitDO(fecbSet.ngFreq); for (b=0;b<2;b++) { digWord[ 2 + b + (j*16)] = bitD0[b]; }
                         // FE SEL D4
                           digWord[4 + (j * 16) ] = fecbSet.rfcmSW;
                          // Walsh Pattern D5,6,7
                             getbitDO(fecbSet.walshPat); for (b=0;b<3;b++) { digWord[ 5 + b + (j*16)] = bitD0[b]; }
                           // Walsh Freq D8,9
                            getbitDO(fecbSet.walshFreq); for (b=0;b<2;b++) { digWord[ 8 + b + (j*16)] = bitD0[b]; }
                          // Spare 10,11
                          digWord[10 + (j*16)] = digWord[11 + (j*16)] = 0;
                          // Walsh Group B12,13,14 freezed to value-1?
                          digWord[12 + (j*16)] = digWord[13 + (j*16)] = digWord[14 + (j*16)] = 1;
                       // bit D15 - EN
                         if(j == 1) digWord[15 + (j*16)] = 1;
                           else digWord[15 + (j*16)] = 0 ;

                       } // for j end
           #ifdef DEBUG
           printf(" INITIALIZING WORDS");
           #endif
          mcm4word[0] = 0 ;  mcm4word[1] = 0;

          for(i = 0 ; i < 16; i++)
          {
             if(digWord[i+16]) mcm4word[0] = mcm4word[1] | (1 << i);
             if(digWord[i]) mcm4word[1] = mcm4word[0] | (1 << i );
          }

        //  printf(" MCM4WORD1 ");
          for(i=0 ; i<16;i++)   {
             if( (mcm4word[1] & ( 1 << i )) ) { b = 1 ; }
             else b = 0 ;
             //printf( "%d", b);
          }
         // printf("\n");

         // printf(" MCM4WORD0 ");
          for(i= 0 ; i<16;i++)  {
             if( (mcm4word[0] & ( 1 << i )) ) { b = 1 ; }
             else b = 0 ;
             //printf( "%d", b);
          }
        //  printf("\n");

        //  printf("################# %04xX %04xX ======> mcm4word 0 and 1 \n", (mcm4word[0] & (~(1 << 4)) ), (mcm4word[1] & (~(1 <<4))) );

}


int  mcmPow(int mcmSW)
{
    int FEDAT_ON, FEDAT_OFF;
    int i,b;

    exeCmd.McmPow = mcmSW;

    switch(mcmSW)
    {
       case 0 : // MCM OFF
               //printf("####### I am in MCMPOW OFF CASE\n");
               sprintf(mask1,"%04x",mcm4word[1] & (~(unsigned short)( 1 << 4)) );
               sprintf(mask2,"%04x",mcm4word[0] & (~(unsigned short)( 1 << 4)) );
 #ifdef DEBUG
               printf("MCMC OFF mask1 %s ", mask1);
               printf("MCM OFF  mask2: %s \n", mask2);
 #endif
               Mask32toArg(mask1, mask2);
               exeCmd.CmdNum = 7;
               IFLO_CTRL(MCMCARD5);  
               usleep(10000);         //set dig mask 32 bits
               break;

       case 1 : 
               sprintf(mask1,"%04x",mcm4word[1] & (~(unsigned short)( 1 << 4)) );
               sprintf(mask2,"%04x",mcm4word[0] & (~(unsigned short)( 1 << 4)) );
#ifdef DEBUG
               printf("MCM OFF mask1: %s ", mask1);
               printf("MCM OFF mask2: %s \n", mask2);
#endif
               Mask32toArg(mask1, mask2);
               exeCmd.CmdNum = 7;
               IFLO_CTRL(MCMCARD5);           //set dig mask 32 bits
                 usleep(10000); 
            
               sprintf(mask1,"%04x",mcm4word[1] | ( 1 << 4) );
               sprintf(mask2,"%04x",mcm4word[0] | ( 1 << 4) );
#ifdef DEBUG
               printf("MCM ON mask1: %s ", mask1);
               printf("MCM ON mask2: %s \n", mask2);
#endif
               Mask32toArg(mask1, mask2);
               exeCmd.CmdNum = 7;
               IFLO_CTRL(MCMCARD5);   usleep(10000); 
               //set dig mask 32 bits
               break;
   }

}


void set_ofcattn()
{
   mask_16bit=0;
   int ch1_val=1, ch2_val=1; 
   ch1_val=ofc_ch1,  ch2_val=ofc_ch2; // Assign the Value received on Ethernet
  // char mask1[12];

  if ( (ch1_val > 15) || ch1_val == 0 )
  { /*printf("Valid range for OF Attenuation setting is 1-15 dB \n");
   printf("Now Setting OF Attenuation to 1 dB \n"); */
   ch1_val = 1; 
   }
  if ( (ch2_val > 15) || ch2_val == 0)
  { /* printf("Valid range for OF Attenuation setting is 1-15 dB \n");
   printf("Now Setting OF Attenuation to 1 dB \n");*/
   ch2_val = 1;
   }
   fprintf(stderr, " ch1_val [%d] ch2_val [%d] \n", ch1_val, ch2_val);
  ch1_val &= 0x1f;
  ch2_val &= 0x1f;

  //printf("Ch 2 0x%2x \n",ch2_val);
  //D0-D5 bit are set to HIGH. This will avoid unnessesary MCM Touts
  //D6-D7 are not used OPTO COUPLER Outputs

  mask_16bit |= 0x003f;
  //printf("mask _16bit = 0x%2x \n",mask_16bit); // D5-D0
  mask_16bit |= (ch1_val <<8);
  //printf("mask _16bit = 0x%2x \n",mask_16bit); // ch1_val is D11-D8

  mask_16bit |= (ch2_val <<12);
  //printf("mask _16bit = 0x%2x \n",mask_16bit); //ch2_val is D15-D12


  /*  printf("Mask for setting OF Attenuation for CH1 and CH2 is %4x Hex \n",mask_16bit);


    printf("*\n");
    printf(" Run File for setting OF Attenuation for CH1 and CH2\n");
    printf("* CH1 OF Attenuation is %2d \n",ch1_val);
    printf("* CH2 OF Attenuation is %2d \n",ch2_val);
    printf("*\n\n");

    printf("mpa 1 0 \n");
    printf("addmcm \n\n"); */

    printf("ana 0%xx\n",mask_16bit);
    printf("st16dig(0)\n");
    sprintf(maskofc,"%x",mask_16bit);
    printf(" Value of mask1 is ====> %s\n",maskofc);
    Mask16toArg(maskofc);
    exeCmd.CmdNum = 6;    // Set DMASK command
    IFLO_CTRL(MCMCARD4);   // MCMCARD is MCM 0
    

    // write srv file
  /*  fprintf(f_srv,"mpa 1 0\naddmcm\n");

    fprintf(f_srv,"ana 0%xx\n",mask_16bit);
    fprintf(f_srv,"st16dig(0)\n");

    change_mask = mask_16bit & 0xff3e;
    fprintf(f_srv,"ana 0%xx\n",change_mask);
    fprintf(f_srv,"st16dig(0)\n");

    fprintf(f_srv,"ana 0%xx\n",mask_16bit);
    fprintf(f_srv,"st16dig(0)\n");

    change_mask = mask_16bit & 0xff3d;
    fprintf(f_srv,"ana 0%xx\n",change_mask);
    fprintf(f_srv,"st16dig(0)\n");

    fprintf(f_srv,"ana 0%xx\n",mask_16bit);
    fprintf(f_srv,"st16dig(0)\n");

    fflush(f_srv); fclose(f_srv);

   // write fps file
     */

    
}

void servo_reset()
{
    sprintf(maskofc,"%x",mask_16bit);
    printf(" Value of mask1 is ====> %s\n",maskofc);
    Mask16toArg(maskofc);
    exeCmd.CmdNum = 6;    // Set DMASK command
    IFLO_CTRL(MCMCARD4);  

    change_mask = mask_16bit & 0xff3e;
    sprintf(maskofc,"%x",change_mask);
    printf(" Value of mask1 is ====> %s\n",maskofc);
    Mask16toArg(maskofc);
    exeCmd.CmdNum = 6;    // Set DMASK command
    IFLO_CTRL(MCMCARD4);  

    sprintf(maskofc,"%x",mask_16bit);
    printf(" Value of mask1 is ====> %s\n",maskofc);
    Mask16toArg(maskofc);
    exeCmd.CmdNum = 6;    // Set DMASK command
    IFLO_CTRL(MCMCARD4);    

    change_mask = mask_16bit & 0xff3d;
    sprintf(maskofc,"%x",change_mask);
    printf(" Value of mask1 is ====> %s\n",maskofc);
    Mask16toArg(maskofc);
    exeCmd.CmdNum = 6;    // Set DMASK command
    IFLO_CTRL(MCMCARD4);  
    
    sprintf(maskofc,"%x",mask_16bit);
    printf(" Value of mask1 is ====> %s\n",maskofc);
    Mask16toArg(maskofc);
    exeCmd.CmdNum = 6;    // Set DMASK command
    IFLO_CTRL(MCMCARD4);  
    
}

void fps_reset()
{
    sprintf(maskofc,"%x",mask_16bit);
    printf(" Value of mask1 is ====> %s\n",maskofc);
    Mask16toArg(maskofc);
    exeCmd.CmdNum = 6;    // Set DMASK command
    IFLO_CTRL(MCMCARD4);

    change_mask = mask_16bit & 0xff3b;
    sprintf(maskofc,"%x",change_mask);
    printf(" Value of mask1 is ====> %s\n",maskofc);
    Mask16toArg(maskofc);
    exeCmd.CmdNum = 6;    // Set DMASK command
    IFLO_CTRL(MCMCARD4);  
    
    sprintf(maskofc,"%x",mask_16bit);
    printf(" Value of mask1 is ====> %s\n",maskofc);
    Mask16toArg(maskofc);
    exeCmd.CmdNum = 6;    // Set DMASK command
    IFLO_CTRL(MCMCARD4); 

 }

// LOFSW system Added on 1/4/2019 
int set_lo()
{
   // LO setting 
     int lofreq,vco_no,tmp1,stp,frq,bitset,tmp2,temp=0,lofreq1,lofreq2;
     char mask[5][8],mask1[5][8],mode[80],maska[2][8],mask1a[2][8],newmask[2][8];
     char permission,ans[10],maskp[2][8],mask1p[2][8];
    // char masksp[6][8] = {"0080","8080","1000","9000","600D","E01D"};  // It is as per OLD LO => Matched with Current Online 
     char masksp[6][8] = {"0880","8880","1000","9000","600D","E01D"};    // It is as per LOFSW  => Matched with Current Online 
     //  char masksp[6][8] = {"00080","08080","01000","09000","0600D","0E01D"};
     int mnum,nnum,mdec,ndec,bx,acum,acum1,acum2,acum3,acuma,acump,yigsc,cnt=2,maxfrq;
   /*  int MASK1=61440;
     int MASK2=4095; */
      int MASK1=0xf000;
      int MASK2=0x0fff;



  			lofreq1 = ifloSet.lo[0];
			lofreq2 = ifloSet.lo[1];
			maxfrq = (lofreq2 >= lofreq1) ? lofreq2 : lofreq1;


   			 if (lofreq1 < 100 || lofreq1 > 1795 || lofreq2 < 100 || lofreq1 > 1795)
				{
    					   // error (4);
    					    return 1;
				}

   				/* if ( 100<=lofreq1 && lofreq1<110 || 190<lofreq1 && lofreq1<220 )
				 {
    				    do    {
						   printf("LOFREQ1 can not be guaranteed.\n");
						   printf("Do you wish to continue?(y/n)");
		  				 scanf("\n%c",&permission);
	     				 }
         				   while(permission !='y' && permission !='n');
      						  if (permission == 'n')
       					     {
       						     printf("Abnormal exit...\n");
       						     return 1;
           				     }
				}
			if( 100<=lofreq2 && lofreq2<110 || 190<lofreq2 && lofreq2<220)
        		    {
       				 do    {
		 			  printf("LOFREQ2 can not be guaranteed.\n");
		  				 printf("Do you wish to continue?(y/n)");
		  			 scanf("\n%c",&permission);
	      				}
           			 while(permission !='y' && permission !='n');
       				 if (permission == 'n')
         			   {
         				   printf("Abnormal exit...\n");
         				   return 1;
          			  }
				} */

				if( (lofreq1 != lofreq2) && ((lofreq1 <= 600 && lofreq2 <= 600) ||
				     (lofreq1 >= 605 && lofreq2 >= 605))  )
				{
					printf("\nSingal VCO cannot support two diffrent Frequencies ");
					//error(4);
					return(1);
				}

  		 if(((lofreq1>=355) && (((float)lofreq1/5-(int)(lofreq1/5)) != 0))
  		 || ((lofreq2>=355) && (((float)lofreq2/5-(int)(lofreq2/5)) != 0)))
			{
				    printf("For VCO 3 and VCO 4, lofreq only in steps of 5MHz.\n");
				    //error(4);
				    return(1);
			}

		lofreq = lofreq1;
		while(cnt--)
		{

			if(lofreq >= 100 && lofreq <= 200) {vco_no = 1;stp = 1;}
			if(lofreq >= 201 && lofreq <= 354) {vco_no = 2;stp = 1;}
			if(lofreq >= 355 && lofreq <= 600) {vco_no = 3;stp = 5;}
			if(lofreq >= 601 && lofreq <= 1800){vco_no = 4;stp = 5;}
			yigsc = 25;

   			 if(vco_no == 1 || vco_no == 4)
   			     {
				sprintf(mask[4], "6005");
				sprintf(mask1[4],"E015");
     			   }
    			if(vco_no == 2 || vco_no == 3)
    			    {
				sprintf(mask[4], "6002");
				sprintf(mask1[4],"E012");
    			    }

   				 if((vco_no == 1) && ( lofreq1 <= lofreq2))
      				  {
					sprintf(mask[2], "0568");
					sprintf(mask1[2],"8568");
					sprintf(mask[3], "1001");
					sprintf(mask1[3],"9001");
       				 }
   			 if((vco_no == 1) && ( lofreq1 > lofreq2))
      			  {
      				  sprintf(mask[2], "09a8");
      				  sprintf(mask1[2],"89a8");
      				  sprintf(mask[3], "1001");
     				  sprintf(mask1[3],"9001");
     			   }
    
   				 if((vco_no == 2) && (lofreq1 <= lofreq2)) 
    				    {
					sprintf(mask[2], "0546");
					sprintf(mask1[2],"8546");
					sprintf(mask[3], "1002");
					sprintf(mask1[3],"9002");
      				  }
    				 if((vco_no == 2) && (lofreq1 > lofreq2))  
    				    {
     					   sprintf(mask[2], "0986");
     	 				   sprintf(mask1[2],"8986");
       					   sprintf(mask[3], "1002");
                                           sprintf(mask1[3],"9002");
                                   }
    
                          if((vco_no == 3) && (lofreq1 <= lofreq2))
                              {
	                         sprintf(mask[2], "0645");
	                         sprintf(mask1[2],"8645");
	                         sprintf(mask[3], "1004");
                                 sprintf(mask1[3],"9004");
                             }
                         if((vco_no == 3) && (lofreq1 > lofreq2)) 
                          {
                               sprintf(mask[2], "0a85");
                               sprintf(mask1[2],"8a85");
                               sprintf(mask[3], "1004");
                               sprintf(mask1[3],"9004");
                         }
    
                     if((vco_no == 4) && (lofreq1 <= lofreq2)) 
                      {
	                    sprintf(mask[2], "0658");
	                    sprintf(mask1[2],"8658");
	                    sprintf(mask[3], "1008");
	                    sprintf(mask1[3],"9008");
                       }
                      if((vco_no == 4) && (lofreq1 > lofreq2))  
                      {
                        sprintf(mask[2], "0a98");
                        sprintf(mask1[2],"8a98");
                        sprintf(mask[3], "1008");
                        sprintf(mask1[3],"9008");
                     }
 

                    if((lofreq1 == lofreq2) && (vco_no != 4))
	            strcpy(mode,"SYN1A->CH1LO,SYN1B->CH2LO");
                  if((lofreq1 == lofreq2) && (vco_no == 4))
	          strcpy(mode,"SYN2B->CH1LO,SYN2A->CH2LO");
                if(lofreq1 < lofreq2)
	           strcpy(mode,"SYN1A->CH1LO,SYN2A->CH2LO");
                if(lofreq1 > lofreq2)
	           strcpy(mode,"SYN2B->CH1LO,SYN1B->CH2LO");


    frq = lofreq/stp;
    mdec=frq-10*((int)(frq/10));
    mnum=15-mdec;
    ndec=((int)(frq/10));
    nnum=255-ndec+1;
    if(mdec >= ndec)
        {
	    printf("ERROR:%d MHz LO is Technically not feasible\n",lofreq);
        return 1;
        }


    bx=nnum*16;
    acum=bx+mnum;
     
	if(vco_no == 4 ) {
                acum = 4095 - acum;
		sprintf(mask[0], "4%03X",acum);
		sprintf(mask1[0],"C%03X",acum);
         } else {
                acum1 = acum & MASK1;
                acum2 = acum ^ MASK2;
                acum3 = acum1 | acum2;
               /*
                printf("04%03X 04%03X\n \n", acum, acum3);
		sprintf(mask[0], "04%03X",acum3);
		sprintf(mask1[0],"0C%03X",acum3);
               *  
               * Jan 2017     
               */ 
                printf("4%03X 4%03X\n \n", acum, acum3);
		sprintf(mask[0], "4%03X",acum3);
		sprintf(mask1[0],"C%03X",acum3);
               
         }


    bx=255*16;
    acum=bx+15;

    if(vco_no == 4)
    {
	bitset = (int)(lofreq-200)/yigsc;
/*	if(sc == 1) bitset--;
	if(sc == 3) bitset++; */
	temp = 0;
	if(lofreq <= 1070) temp = 192;
	if(lofreq >= 1075 && lofreq <= 1100) temp = 64;
	if(lofreq > 1100 && lofreq <= 1200) temp = 128;
	acum = bitset + 3840 + temp;
	acuma = bitset + 1 + 3840 + temp;
	acump = bitset - 1 + 3840 + temp;
    }

    sprintf(mask[1], "2%03X",acum);
    sprintf(mask1[1],"A%03X",acum);
    if(vco_no == 4)
    {
    sprintf(maska[1], "2%03X",acuma);
    sprintf(mask1a[1],"A%03X",acuma);
    sprintf(maskp[1], "2%03X",acump);
    sprintf(mask1p[1],"A%03X",acump);
    }



    if((lofreq1 == lofreq2) && (cnt == 1))
       {
          if(vco_no == 4) 
            {
                       Mask64toArg(masksp[0], masksp[1], masksp[2], masksp[3]);
                       exeCmd.CmdNum = 8;
                       IFLO_CTRL(MCMCARD1);   //MCM 2 
                       Mask32toArg(masksp[4],masksp[5]);
                       exeCmd.CmdNum = 7;
                       IFLO_CTRL(MCMCARD1); // MCM 2
              
            }
          else 
          {
             
               Mask64toArg(masksp[0], masksp[1], masksp[2], masksp[3]);
               exeCmd.CmdNum = 8;
               IFLO_CTRL(MCMCARD2); //MCM 3 
               Mask32toArg(masksp[4],masksp[5]);
               exeCmd.CmdNum = 7;
               IFLO_CTRL(MCMCARD2); // MCM 3
          } 
            sprintf(newmask[0], "3%03X",lofreq);
            sprintf(newmask[1], "B%03X",lofreq);
           if(vco_no == 4) 
            {
               Mask32toArg(newmask[0],newmask[1]);
               exeCmd.CmdNum = 7;  IFLO_CTRL(MCMCARD2); // MCM 3
            }
	    else
             {
                Mask32toArg(newmask[0],newmask[1]);
                exeCmd.CmdNum = 7;  IFLO_CTRL(MCMCARD1); // MCM 2
             }
     }
    else
     {

            if(vco_no == 4)
             {
                
                  Mask32toArg(masksp[2],masksp[3]);
                  exeCmd.CmdNum = 7;
                  IFLO_CTRL(MCMCARD2); // MCM 3 
                  Mask64toArg(mask[0], mask1[0], mask[1], mask1[1]);
                  exeCmd.CmdNum = 8;
                  IFLO_CTRL(MCMCARD2); // MCM 3 
               }
              else 
             {    
       
                  Mask32toArg(masksp[2],masksp[3]);exeCmd.CmdNum = 7;
                  IFLO_CTRL(MCMCARD1); // MCM 2
                  Mask64toArg(mask[0], mask1[0], mask[1], mask1[1]);
                  exeCmd.CmdNum = 8;   // MCM 2
                  IFLO_CTRL(MCMCARD1);
             }

       /* Commented after discussion with Jitu bhai 5/4/2019 */ 
        /*    if(maxfrq > 600 && vco_no == 4)
          {
                 
              Mask32toArg(maska[1], mask1a[1]);
              exeCmd.CmdNum = 7;  IFLO_CTRL(MCMCARD2); // MCM 3
              Mask32toArg(maskp[1], mask1p[1]);
              exeCmd.CmdNum = 7;  IFLO_CTRL(MCMCARD2); // MCM 3

           } */

    
           if(vco_no == 4)  
            {     
               
                Mask64toArg(mask[2], mask1[2], mask[3], mask1[3]);exeCmd.CmdNum = 8;
                  IFLO_CTRL(MCMCARD2); // MCM 3
                 Mask32toArg(mask[4], mask1[4]); exeCmd.CmdNum = 7;
                  IFLO_CTRL(MCMCARD2); // MCM 3 
            }
           else 
             {  
                
                  Mask64toArg(mask[2], mask1[2], mask[3], mask1[3]); exeCmd.CmdNum = 8;
                  IFLO_CTRL(MCMCARD1); // MCM 2 
                  Mask32toArg(mask[4], mask1[4]); exeCmd.CmdNum = 7;
                  IFLO_CTRL(MCMCARD1); // MCM 2 
             }
           
            sprintf(newmask[0], "3%03X",lofreq);
            sprintf(newmask[1], "B%03X",lofreq);
	    if(vco_no == 4) 
            {
               Mask32toArg(newmask[0],newmask[1]);
               exeCmd.CmdNum = 7;  IFLO_CTRL(MCMCARD2); // MCM 3
            }
	    else
             {
                Mask32toArg(newmask[0],newmask[1]);
                exeCmd.CmdNum = 7;  IFLO_CTRL(MCMCARD1); // MCM 2
             }
          
         } 
               if(cnt == 1) lofreq = lofreq2;
      }
} 


// OLD LO System 
/* int set_lo()
{
   // LO setting 
     int lofreq,vco_no,tmp1,stp,frq,bitset,tmp2,temp=0,lofreq1,lofreq2;
     char mask[5][8],mask1[5][8],mode[80],maska[2][8],mask1a[2][8];
     char permission,ans[10],maskp[2][8],mask1p[2][8];
     char masksp[6][8] = {"00080","08080","01000","09000","0600D","0E01D"};
     int mnum,nnum,mdec,ndec,bx,acum,acum1,acum2,acum3,acuma,acump,yigsc,cnt=2,maxfrq;
     int MASK1=61440;
     int MASK2=4095;


  			lofreq1 = ifloSet.lo[0];
			lofreq2 = ifloSet.lo[1];
			maxfrq = (lofreq2 >= lofreq1) ? lofreq2 : lofreq1;


   			 if (lofreq1 < 100 || lofreq1 > 1795 || lofreq2 < 100 || lofreq1 > 1795)
				{
    					   // error (4);
    					    return 1;
				}

   				 if ( 100<=lofreq1 && lofreq1<110 || 190<lofreq1 && lofreq1<220 )
				 {
    				    do    {
						   printf("LOFREQ1 can not be guaranteed.\n");
						   printf("Do you wish to continue?(y/n)");
		  				 scanf("\n%c",&permission);
	     				 }
         				   while(permission !='y' && permission !='n');
      						  if (permission == 'n')
       					     {
       						     printf("Abnormal exit...\n");
       						     return 1;
           				     }
				}
			if( 100<=lofreq2 && lofreq2<110 || 190<lofreq2 && lofreq2<220)
        		    {
       				 do    {
		 			  printf("LOFREQ2 can not be guaranteed.\n");
		  				 printf("Do you wish to continue?(y/n)");
		  			 scanf("\n%c",&permission);
	      				}
           			 while(permission !='y' && permission !='n');
       				 if (permission == 'n')
         			   {
         				   printf("Abnormal exit...\n");
         				   return 1;
          			  }
				}

				if( (lofreq1 != lofreq2) && ((lofreq1 <= 600 && lofreq2 <= 600) ||
				     (lofreq1 >= 605 && lofreq2 >= 605))  )
				{
					printf("\nSingle SYNTH cannot support two diffrent Frequencies\n\n");
					//error(4);
					return(1);
				}

  		 if(((lofreq1>=355) && (((float)lofreq1/5-(int)(lofreq1/5)) != 0))
  		 || ((lofreq2>=355) && (((float)lofreq2/5-(int)(lofreq2/5)) != 0)))
			{
				    printf("For VCO 3 and VCO 4, lofreq only in steps of 5MHz.\n");
				    //error(4);
				    return(1);
			}

		lofreq = lofreq1;
		while(cnt--)
		{

			if(lofreq >= 100 && lofreq <= 200) {vco_no = 1;stp = 1;}
			if(lofreq >= 201 && lofreq <= 354) {vco_no = 2;stp = 1;}
			if(lofreq >= 355 && lofreq <= 600) {vco_no = 3;stp = 5;}
			if(lofreq >= 601 && lofreq <= 1800){vco_no = 4;stp = 5;}
			yigsc = 25;

   			 if(vco_no == 1 || vco_no == 4)
   			     {
				sprintf(mask[4], "6005");
				sprintf(mask1[4],"E015");
     			   }
    			if(vco_no == 2 || vco_no == 3)
    			    {
				sprintf(mask[4], "6002");
				sprintf(mask1[4],"E012");
    			    }

   				 if((vco_no == 1) && ( lofreq1 <= lofreq2))
      				  {
					sprintf(mask[2], "0168");
					sprintf(mask1[2],"8168");
					sprintf(mask[3], "1001");
					sprintf(mask1[3],"9001");
       				 }
   			 if((vco_no == 1) && ( lofreq1 > lofreq2))
      			  {
      				  sprintf(mask[2], "01a8");
      				  sprintf(mask1[2],"81a8");
      				  sprintf(mask[3], "1001");
     				  sprintf(mask1[3],"9001");
     			   }
    
   				 if((vco_no == 2) && (lofreq1 <= lofreq2)) 
    				    {
					sprintf(mask[2], "0146");
					sprintf(mask1[2],"8146");
					sprintf(mask[3], "1002");
					sprintf(mask1[3],"9002");
      				  }
    				 if((vco_no == 2) && (lofreq1 > lofreq2))  
    				    {
     					   sprintf(mask[2], "0186");
     	 				   sprintf(mask1[2],"8186");
       					   sprintf(mask[3], "1002");
                                           sprintf(mask1[3],"9002");
                                   }
    
                          if((vco_no == 3) && (lofreq1 <= lofreq2))
                              {
	                         sprintf(mask[2], "0245");
	                         sprintf(mask1[2],"8245");
	                         sprintf(mask[3], "1004");
                                 sprintf(mask1[3],"9004");
                             }
                         if((vco_no == 3) && (lofreq1 > lofreq2)) 
                          {
                               sprintf(mask[2], "0285");
                               sprintf(mask1[2],"8285");
                               sprintf(mask[3], "1004");
                               sprintf(mask1[3],"9004");
                         }
    
                     if((vco_no == 4) && (lofreq1 <= lofreq2)) 
                      {
	                    sprintf(mask[2], "0258");
	                    sprintf(mask1[2],"8258");
	                    sprintf(mask[3], "1008");
	                    sprintf(mask1[3],"9008");
                       }
                      if((vco_no == 4) && (lofreq1 > lofreq2))  
                      {
                        sprintf(mask[2], "0298");
                        sprintf(mask1[2],"8298");
                        sprintf(mask[3], "1008");
                        sprintf(mask1[3],"9008");
                     }
 

                    if((lofreq1 == lofreq2) && (vco_no != 4))
	            strcpy(mode,"SYN1A->CH1LO,SYN1B->CH2LO");
                  if((lofreq1 == lofreq2) && (vco_no == 4))
	          strcpy(mode,"SYN2B->CH1LO,SYN2A->CH2LO");
                if(lofreq1 < lofreq2)
	           strcpy(mode,"SYN1A->CH1LO,SYN2A->CH2LO");
                if(lofreq1 > lofreq2)
	           strcpy(mode,"SYN2B->CH1LO,SYN1B->CH2LO");


    frq = lofreq/stp;
    mdec=frq-10*((int)(frq/10));
    mnum=15-mdec;
    ndec=((int)(frq/10));
    nnum=255-ndec+1;
    if(mdec >= ndec)
        {
	    printf("ERROR:%d MHz LO is Technically not feasible\n",lofreq);
        return 1;
        }


    bx=nnum*16;
    acum=bx+mnum;
     
	if(vco_no == 4 ) {
                acum = 4095 - acum;
		sprintf(mask[0], "4%03X",acum);
		sprintf(mask1[0],"C%03X",acum);
         } else {
                acum1 = acum & MASK1;
                acum2 = acum ^ MASK2;
                acum3 = acum1 | acum2;
               
               // printf("04%03X 04%03X\n \n", acum, acum3);
		//sprintf(mask[0], "04%03X",acum3);
		//sprintf(mask1[0],"0C%03X",acum3);
                 
               // Jan 2017     
               
                printf("4%03X 4%03X\n \n", acum, acum3);
		sprintf(mask[0], "4%03X",acum3);
		sprintf(mask1[0],"C%03X",acum3);
               
         }


    bx=255*16;
    acum=bx+15;

    if(vco_no == 4)
    {
	bitset = (int)(lofreq-200)/yigsc;
//	if(sc == 1) bitset--;
//	if(sc == 3) bitset++; 
	temp = 0;
	if(lofreq <= 1070) temp = 192;
	if(lofreq >= 1075 && lofreq <= 1100) temp = 64;
	if(lofreq > 1100 && lofreq <= 1200) temp = 128;
	acum = bitset + 3840 + temp;
	acuma = bitset + 1 + 3840 + temp;
	acump = bitset - 1 + 3840 + temp;
    }

    sprintf(mask[1], "2%03X",acum);
    sprintf(mask1[1],"A%03X",acum);
    if(vco_no == 4)
    {
    sprintf(maska[1], "2%03X",acuma);
    sprintf(mask1a[1],"A%03X",acuma);
    sprintf(maskp[1], "2%03X",acump);
    sprintf(mask1p[1],"A%03X",acump);
    }



    if((lofreq1 == lofreq2) && (cnt == 1))
       {
          if(vco_no == 4) 
            {
                       Mask64toArg(masksp[0], masksp[1], masksp[2], masksp[3]);
                       exeCmd.CmdNum = 8;
                       IFLO_CTRL(MCMCARD1);
                       Mask32toArg(masksp[4],masksp[5]);
                       exeCmd.CmdNum = 7;
                       IFLO_CTRL(MCMCARD1);
              
            }
          else 
          {
             
               Mask64toArg(masksp[0], masksp[1], masksp[2], masksp[3]);
               exeCmd.CmdNum = 8;
               IFLO_CTRL(MCMCARD2);
               Mask32toArg(masksp[4],masksp[5]);
               exeCmd.CmdNum = 7;
               IFLO_CTRL(MCMCARD2);
       } 
     }
    else
     {

            if(vco_no == 4)
             {
                
                  Mask32toArg(masksp[2],masksp[3]);
                  exeCmd.CmdNum = 7;
                  IFLO_CTRL(MCMCARD2);
                  Mask64toArg(mask[0], mask1[0], mask[1], mask1[1]);
                  exeCmd.CmdNum = 8;
                  IFLO_CTRL(MCMCARD2);
               }
              else 
             {    
       
                  Mask32toArg(masksp[2],masksp[3]);exeCmd.CmdNum = 7;
                  IFLO_CTRL(MCMCARD1);
                  Mask64toArg(mask[0], mask1[0], mask[1], mask1[1]);
                  exeCmd.CmdNum = 8;
                  IFLO_CTRL(MCMCARD1);
             }

          // Added on 12/01/2015 
         //   if(maxfrq > 600 && vco_no == 4)
        //  {
           //       *buf_ptr0= 3;
            //     Mask32toArg(buf_ptr0,maska[1], mask1a[1]);
            //   Mask32toArg(buf_ptr0,maskp[1], mask1p[1]);

        //   }

    
           if(vco_no == 4)  
            {     
               
                Mask64toArg(mask[2], mask1[2], mask[3], mask1[3]);exeCmd.CmdNum = 8;
                  IFLO_CTRL(MCMCARD2);
                Mask32toArg(mask[4], mask1[4]); exeCmd.CmdNum = 7;
                  IFLO_CTRL(MCMCARD2);
            }
           else 
             {  
                
                  Mask64toArg(mask[2], mask1[2], mask[3], mask1[3]); exeCmd.CmdNum = 8;
                  IFLO_CTRL(MCMCARD1);
                  Mask32toArg(mask[4], mask1[4]); exeCmd.CmdNum = 7;
                  IFLO_CTRL(MCMCARD1);
             }
         
          
         } 
               if(cnt == 1) lofreq = lofreq2;
      }
} */

void set_if()
{

   int d[4],bw1,bw2;
    int set_para[23];
    char mask[4][8];    
    int pre_att,pst_att,bw,alc=0,alc1,alc2,Alc;
    int lsb1,lsb2,xgain_ch1,xgain_ch2,fbv_dec1,fbv_dec2;
    float post_gain[2];
    
    int i,j;
  
    int p_bw2[]     = {0xc8, 0xd0, 0xe0};
    int p_bw1[]     = {0x01, 0x02, 0x04};
    int band_width[]= {   6,   16,   32, -1};
 
      set_para[0]= ifloSet.if_pa[0];
      set_para[2]= ifloSet.if_pa[1];
      post_gain[0]= ifloSet.if_pg[0];
      post_gain[1]= ifloSet.if_pg[1];
      set_para[4]= ifloSet.if_band[0];
      set_para[5]= ifloSet.if_band[1];
      set_para[6]= ifloSet.if_alc[0];
      set_para[7]= ifloSet.if_alc[1];
    // printf("#######==== >>>> SET PARA %d %d %f %f %d %d %d %d\n", set_para[0],set_para[2],post_gain[0],post_gain[1],set_para[4],set_para[5],set_para[6],set_para[7]);
     
    for (i=0,j=0; i<4; i++,j++)
    {
        if(set_para[i]<0 || 30<set_para[i])
        {
           // error(5);
          //  return 1;
        }
        d[j]=set_para[i];
        d[j]=(d[j])/2;

        if (d[j]<=3 || (d[j]>=8 && d[j]<=11))/*to invert second MSB*/
            d[j]=d[j]+4;
        else
            d[j]=d[j]-4;

        if (j>=2)     /* j>=2  ???? */
            d[j]=d[j]*16;
    }
/* Pre and Post Attn. get set for both Channels SIMULTANEOUSLY */
    pre_att= d[0]+d[2]; /* d[0] refers to CH2 (175 MHz),d[2] to CH1 */
   // pst_att= d[1]+d[3];

/*     control word for IF BW setting.                             */

    for(i=0; band_width[i]!=-1 && set_para[4]!=band_width[i]; i++);
    if(band_width[i] == -1)
        {
       // error(9);
       // return 1;
        }
    bw1 = p_bw1[i];
    for(i=0; band_width[i]!=-1 && set_para[5]!=band_width[i]; i++);
    if(band_width[i] == -1)
        {
       // error(10);
      //  return 1;
        }
    bw2 = p_bw2[i];
    bw = bw2 + bw1;

/*    control word for ALC ON/OFF.                               */

    if((set_para[6]==0)&&(set_para[7]==0))
        alc=0xcc;

    if((set_para[6]==0)&&(set_para[7]==1))
        alc=0xfd;

    if((set_para[6]==1)&&(set_para[7]==0))
        alc=0xfe;

    if((set_para[6]==1)&&(set_para[7]==1))
        alc=0xff;

    sprintf(mask[0], "ff%02xx",alc);
    sprintf(mask[1], "ff%02xx",alc);
    sprintf(mask[2], "bf%02xx",alc);
    sprintf(mask[3], "ff%02xx",alc);
   
    Mask64toArg(mask[0], mask[1], mask[2], mask[3]);
    exeCmd.CmdNum = 8;
    IFLO_CTRL(MCMCARD3);

    sprintf(mask[0], "b3%02xx",pre_att);
    sprintf(mask[1], "f3%02xx",pre_att);
    sprintf(mask[2], "b3%02xx",pre_att);
    sprintf(mask[3], "f3%02xx",pre_att);
    
    Mask64toArg(mask[0], mask[1], mask[2], mask[3]);
    exeCmd.CmdNum = 8;
    IFLO_CTRL(MCMCARD3);
    bzero(mask,sizeof(mask));

  float gain_ch1=post_gain[0], gain_ch2=post_gain[1];
  alc1=set_para[6]; alc2=set_para[7];

   if(gain_ch1>35 || gain_ch1<-5)
        {
        printf("Set Gain of CH-1 in between -5 dB to 35 dB\n");
        exit(1);
        }
        else if(((gain_ch1-(int) gain_ch1)!=0.5)&&((gain_ch1-(int) gain_ch1)!=0.0))
        {
        printf("Set Gain of CH-1 in multiples of 0.5 dB\n");
        exit(1);
        }

        if(gain_ch2>35 || gain_ch2<-5)
        {
        printf("Set Gain of CH-2 in between -5 dB to 35 dB\n");
        exit(1);
        }
        else if(((gain_ch2-(int) gain_ch2)!=0.5)&&((gain_ch2-(int) gain_ch2)!=0.0))
        {
        printf("Set Gain of CH-2 in multiples of 0.5 dB\n");
        exit(1);
        }
     if(alc1>1 || alc1<0)
        {
        printf("Enter Proper Bit (0-OFF/ 1-ON) for CH-1 ALC Switching\n");
        exit (1);
        }
        else if(alc2>1 || alc2<0)
                {
                printf("Enter Proper Bit (0-OFF/ 1-ON) for CH-2 ALC Switching\n");
                exit (1);
                }
        Alc = alc1 | alc2<<1;
        xgain_ch1 = 90 - (2*gain_ch1);
        xgain_ch2 = 90 - (2*gain_ch2);
        lsb1 = xgain_ch1 & 0x7;
        lsb2 = xgain_ch2 & 0x7;
        lsb1 = ((lsb2<<5) | (lsb1<<2));
        lsb1 = lsb1 | Alc;

  sprintf(mask[0], "b7%x%xx",xgain_ch2>>3&0xf, xgain_ch1>>3&0xf );
  sprintf(mask[1], "f7%x%xx", xgain_ch2>>3&0xf, xgain_ch1>>3&0xf);
  sprintf(mask[2], "bf%x%xx", (lsb1>>4)&0xf,lsb1&0xf);
  sprintf(mask[3], "ff%x%xx", (lsb1>>4)&0xf, lsb1&0xf); 
  
   Mask64toArg(mask[0], mask[1], mask[2], mask[3]);
   exeCmd.CmdNum = 8;
   IFLO_CTRL(MCMCARD3);

/* IF Attn setting done */

    sprintf(mask[0], "fb%02xx",bw);
    sprintf(mask[1], "fb%02xx",bw);
    sprintf(mask[2], "bb%02xx",bw);
    sprintf(mask[3], "fb%02xx",bw);
   
    Mask64toArg(mask[0], mask[1], mask[2], mask[3]);
     exeCmd.CmdNum = 8;
     IFLO_CTRL(MCMCARD3);
}


int ifloCtrlCmd(parseCMSCmd *Wrpr_Cmd)
{
     
   typedef enum { SETLO=0, SETIF=1, OFCATTN=2,DOMON=3,SRVRST=4,FPSRST=5,SETSIGCON=6,SETIFATTN=7,WALSHPATERN=8, SETWALSHGRP=9,RFCM_SW=10, 
                  WALSHFREQ=11,NOISEFREQ=12,SETRFNOISE=13,SETWALSH=14, TCMD=15 } IFLOEnum;
     resp_msg_cntr = 0 ;

        

       switch (ifloSet.cmdCode)
        {
            case SETLO : 
                      	  set_lo();
                          printf(" =====>>> SET LO completed================>\n");
 			   break ;

      	    case SETIF :    set_if();
                            printf(" =====>>> SET IF completed\n");
			    break;  

            case OFCATTN :
                           set_ofcattn();
                           printf(" =====>>> SET OFC ATTN completed\n");
                           break;

           case DOMON  :  set_ofcattn();
                          printf(" =====>>> DOMON completed\n");
             
                           break;
           case SRVRST  : 
                           servo_reset();
                           break;
           case FPSRST  :
                           fps_reset();
                           break;

          case SETSIGCON :  set_lo();
                            printf(" =====>>> SET LO completed================>\n");
                            set_if();
                            printf(" =====>>> SET IF completed\n");
			    break; 

          case SETIFATTN :  set_if();
                            break;

           case WALSHPATERN:  mcm4setting();printf(" =====>>> Walsh pattern completed================>\n");
                             mcmPow(1);printf(" =====>>> Walsh pattern completed================>\n");
                           break;
          case WALSHFREQ:    mcm4setting(); printf(" =====>>> Walsh freq completed================>\n");
                           mcmPow(1);printf(" =====>>> Walsh frer completed================>\n");
                           break;

          case NOISEFREQ:    mcm4setting(); printf(" =====>>> Noise freq completed================>\n");
                           mcmPow(1);printf(" =====>>> noise freq completed================>\n");
                           break;

          case SETRFNOISE:   mcm4setting(); printf(" =====>>> NG DUTY Cycle completed================>\n");
                           mcmPow(1); printf(" =====>>> NG DUTY Cycle completed================>\n");
                           break;
          case SETWALSHGRP : mcm4setting();   // Actual implementation is not here. In future we can add. 
                            mcmPow(1);
                           break;
          case SETWALSH : mcm4setting();   // Actual implementation is not here. In future we can add. 
                           mcmPow(1);
                           break;
          case RFCM_SW :   mcm4setting(); printf(" =====>>> MCM 5 on completed================>\n");
                           mcmPow(fecbSet.rfcmSW);                     
                        break;
           
    }

}

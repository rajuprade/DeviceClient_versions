/*-----------------------------------------------------------------*/
/* AAGE PICHE modification
   creates LOA.SET and LOP.SET for frequencies > 600 MHz */
/*-----------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COMMENT   '*'
#define LINE_LEN  256

 short bitDO[8];
int set_para[32]; /* Total 30 Fields */
float post_gain[2];
float gab_attn[2];
char PratrCode(int );
// void getbitDO(short , short []);

FILE *f1;
/*-----------------------------------------------------------------*/
    int error(int err)
    {
    printf("\nWRONG VALUE ENTERED FOR ");
    switch(err)
        {
        case 0:  printf("BAND SELECTION\n");   break;
        case 1:  printf("SOLAR ATTENUATOR\n"); break;
        case 3:  printf("CAL NOISE\n");        break;
        case 4:  printf("LO frequency.\n");    break;
        case 5:  printf("IF ATTENUATION\n");   break;
        case 9:  printf("IF BW for CH1\n");    break;
        case 10: printf("IF BW dor CH2\n");    break;
        default: printf("\nABNORMAL EXIT...");
        }
    }
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
    void filter_comment(char *x)
    {
    char *p;
    p=strchr(x, COMMENT);
    if(p!=NULL)
        *p='\0';
    }

/*-----------------------------------------------------------------*/
    int is_empty(char *x)
    {
    int i;

    for(i=0; x[i]==' '||x[i]=='\t'||x[i]=='\n'; i++);
    if (x[i]=='\0')
        return 1;
    else
        return 0;
    /*return x[i]=='\0' ? 1 : 0;*/
    }
/*-----------------------------------------------------------------*/

void getbitDO(short word)
{  
  int i, mask, t_bit;
  extern short bitDO[];

   for(i=7;i >= 0;) 
   {  
     mask = 1<<i;
     t_bit =  word & mask;            /* and operator */
     bitDO[i] =  ( t_bit == 0?0:1 ) ; 
     i--;
   }
#ifdef JDEBUG
    for(i=0;i<7;i++) bitDO[i] == 0?fprintf(stderr,"0"):fprintf(stderr,"1");
    fprintf(stderr, "\n");
#endif

}

int get_int(char *SetLine)
{
       char *temp1;
       char str[10], str_dest[10];
       int lo,len;

       temp1=strtok(SetLine, ".");
       strcpy(str, temp1);
       if(strlen(str) != 2) return 0;	
       temp1=strtok(NULL, "\n");
       strcat(str, temp1);
       if(strlen(str) >= 9) return atoi(strncpy(str_dest, str,8));
       else {
       	lo=atoi(str);
       	while(lo < 9999999) lo*=10;
/*       fprintf(stderr, "Integer lo %s\n", str);	*/
       
        return lo;
       }
}

/*
//-----------------------------------------------------------------//
      generates a run file GAB.SET for GAB parameters.           */

    int set_gab(void)
    {
       int i,j, digmask, t_bit;
       short at_code1, at_code2;
       int CH2Diff=15;
       short digWord[32]; int gabbits; 
       extern short bitDO[];
       
/*------------------- LO Settings ---------------------------*/
    if( (set_para[16] < 600000 || set_para[16] > 1500000 ) || (set_para[17] < 600000 || set_para[17] > 1500000 ) ) {
       fprintf(stderr, "### ERROR GAB : Invalid gab LO value CH1 %d CH2 %d \n", set_para[16], set_para[17]);
       return 1;
    }
    if ( (int) (set_para[16] % 500) ||  (int) (set_para[17] % 500) ) {
       fprintf(stderr, "### ERROR GAB : Invalid gab LO value : Must be in steps of 500 KHz\n");
       return 1;
    } 
    

/*
    if(set_para[16] < 600000) {
        fprintf(stderr, "GAB CH1 LO Below 600 MHz selecting Direct Path \n");
        set_para[22] = 1;
    }

    if(set_para[17] < 600000) {
        fprintf(stderr, "GAB CH2 LO Below 600 MHz selecting Direct Path \n");
        set_para[23] = 1;
    }
*/

/*--------------------< Attenuations >---------------------------*/

    if(gab_attn[0] < 0  || gab_attn[0] > 31.5 )  { fprintf(stderr, "Invalid gab attenuation value .. %f \n", gab_attn[0]); return 1; }
    if(gab_attn[1] < 0  || gab_attn[1] > 31.5 )  { fprintf(stderr, "Invalid gab attenuation value .. %f \n", gab_attn[1]); return 1; }

    at_code1 = 63.0  - ( gab_attn[0] * 2.0 );
    getbitDO(at_code1);
    fprintf(stderr, " %d \n", at_code1);

     for(i=5;i>=0;i--) {
             digWord[i] = bitDO[i]; 

#ifdef JDEBUG
             fprintf(stderr, " %d[%d] - %d[%d] ", i, bitDO[i], i, digWord[i]);
#endif

     }

    at_code2 = 63.0  - ( gab_attn[1] * 2.0 );
    getbitDO(at_code2);

     for(i=5;i>=0;i--) {
             digWord[i+CH2Diff] = bitDO[i]; 
#ifdef JDEBUG
             fprintf(stderr, " %d[%d] - %d[%d] ", i, bitDO[i], i, digWord[i+CH2Diff]);
#endif
     }


 /*--------------------< ANT RF=0, VGA CAL=1, BUF CAL or TST SIGN=2  6, 7, 22, 23 >-------------------*/
     switch(set_para[20])
     {
          case 0 : digWord[6] = digWord[7] = 0 ; break;      
          case 1 : digWord[6] = 1 ; digWord[7] = 0 ; break;
          case 2 : digWord[6] = 0 ; digWord[7] = 1 ; break;
          default: fprintf(stderr, "Warning : Invalid Input, taking default ANT RF for CH1 \n");
                   digWord[6] = digWord[7] = 0 ; break;
     }
     switch(set_para[21])
     {
          case 0 : digWord[6+CH2Diff] = digWord[7+CH2Diff] = 0 ; break;
          case 1 : digWord[6+CH2Diff] = 1 ; digWord[7+CH2Diff] = 0 ; break;
          case 2 : digWord[6+CH2Diff] = 0 ; digWord[7+CH2Diff] = 1 ; break;
          default: fprintf(stderr, "Warning : Invalid Input, taking default ANT RF for CH2 \n");
                   digWord[6+CH2Diff] = digWord[7+CH2Diff] = 0 ;
     }

 /*-----------------< MIXER OR Direct Path D8, D24 >----------------------*/          
    if (set_para[22] == 1) digWord[8]  = 1; else digWord[8]  = 0; 
    if (set_para[23] == 1) digWord[8+CH2Diff] = 1; else digWord[8+CH2Diff]  = 0; 


 /*----------------< Filter RF - 9,10,11 - 24,25, 26>--------------------------------*/
    if(  (set_para[26] < 0 || set_para[26] > 7)|| (set_para[27] < 0 || set_para[27] > 7) ) {
        fprintf(stderr, "Invalid Input for Filter RF CH1 %d CH2 %d \n",set_para[26],set_para[27]);
        return 1;
    }

     getbitDO((short)set_para[26]); for(i=0;i < 3;i++) { digWord[i+9] = bitDO[i];}
     getbitDO((short)set_para[27]); for(i=0;i < 3;i++) { digWord[i+9+CH2Diff] = bitDO[i]; }
           
 /*----------------------< LPF 12, 13, 27 28 >--------------------------*/

    switch(set_para[28]) {
           case 100 : digWord[12] = 0 ;  digWord[13] = 0 ; break;
           case 200 : digWord[12] = 1; digWord[13] = 0 ; break;
           case 400 : digWord[12] = 0; digWord[13] = 1 ; break;
           case   0 : digWord[12] =    digWord[13] = 1 ; break;
           default  : fprintf(stderr, "Invalid LPF CH1 %d taking Direct Path \n", set_para[28]);
                      digWord[12] =    digWord[13] = 0 ; 
    }

    switch(set_para[29]) {
           case 100 : digWord[12+CH2Diff] = 0; digWord[13+CH2Diff] = 0 ; break;
           case 200 : digWord[12+CH2Diff] = 1; digWord[13+CH2Diff] = 0 ; break;
           case 400 : digWord[12+CH2Diff] = 0; digWord[13+CH2Diff] = 1 ; break;
           case   0 : digWord[12+CH2Diff] =    digWord[13+CH2Diff] = 1 ; break;
           default  : fprintf(stderr, "Invalid LPF CH1 %d taking Direct Path \n", set_para[28]);
                      digWord[12+CH2Diff] =    digWord[13+CH2Diff] = 0 ; 
    }

   /*-----------------------< Spare 14 29 >----------------------------*/
    if(set_para[30] == 1) digWord[14]  = 1; else  digWord[14]  = 0; 
    if(set_para[31] == 1) digWord[14+CH2Diff] = 1; else digWord[14+CH2Diff]  = 0; 

 /*-----------------< LO Synthesizer Synthesizer/Siggen 30,31 >----------------------*/          
    if (set_para[24] == 1) digWord[30]  = 1; else digWord[30]  = 0; 
    if (set_para[25] == 1) digWord[31] = 1; else digWord[31]  = 0; 
  

    f1 = fopen("/home/observer/set/user/user1/gab.set", "w");
    if(f1==NULL)
    {
        printf("File opening error.\nAbnormal exit...");
        return 20;
    }

    fprintf(f1,"\n* run file for setting GAB SET parameters.\n");
    fprintf(f1,"\n* SET GAB LO %ld %ld \n",set_para[16],set_para[17]);
    fprintf(f1,"stgablo('%6d'\,'%6d')\n",set_para[16],set_para[17]);

    gabbits = 0;
    for(i=0;i<32;i++) {
       if (digWord[i]) {
           gabbits = gabbits | (1 << i) ;
       }
    }

    fprintf(f1,"\n* GABBITS :");
    for(i=31;i >=0;i--) {
          digmask = 1 << i;
          t_bit = gabbits & digmask;
          t_bit == 0?fprintf(f1,"0"):fprintf(f1,"1");
    } 
    fprintf(f1, "\nstgabconf(\'%.8x\')\n", gabbits);

    fflush(f1);
    fclose(f1);
    return 0;

    }
/*-----------------------------------------------------------------*/
/*    generates a run file FE.SET for FE parameters.           */

    int set_fe(void)
    {
    int data11,data12,data21,data22,data31,data32;
    int data1,data2,data3,add_fe;
    int Walsh_Enbl,FEDAT_ON,FEDAT_OFF;
    short NG_cycle, NG_Freq[2], FE_Sel[2], WLSH_P[2], WLSH_Freq[2], WLSH_Grp[2], ns_cal_index;
    unsigned short mcm4word[2];
    char WE[16],WG[16],SWAP[16];
    FILE *f1;
    int i, b;
    short digWord[32];

    int p_data11[] = {0x0, 0x1, 0x8, 0xa, 0xa,  0xa,  0xa,  0xa,  0x10, 0x10, 0x10, 0x10, 0x10, 0x14, 0x14, 0x14, 0x14, 0x14};
    int p_data32[] = {0x0, 0x0, 0x0,0x60, 0x20, 0x40, 0x0,  0x80, 0x60 ,0x20, 0x80, 0x40, 0x00, 0x00, 0x40, 0x20, 0x60, 0x90};
    int p_add []   = {0x1, 0x2, 0x3, 0x4, 0x4,  0x4,  0x4,  0x4,  0x5 ,  0x5, 0x5,  0x5,   0x5,  0x6,  0x6,  0x6,  0x6, 0x6};
    int freq_band[]= {50, 150, 235,  290, 350,  410,  470,  325,  600, 685, 725, 770, 850,1060, 1170, 1280, 1390, 1420, -1};

    int p_data31[] = {0x06, 0x05, 0x0a, 0x09, 0x15};

    int p_data12[] = {0x80, 0xe0, 0x00, 0x60, 0xa0};
    int p_data21[] = {0x01, 0x01, 0x00, 0x00, 0x00};
    int sol_attr[] = {   0,   14,   30,   44,  -1};

    int ns_cal[]   = {   0,    1,    2,    3, -1};
    char *N_CAL[]   = {"E-HI","HI","MED","LOW","RF-OFF"};

/*    control word for BAND SELECTION & RF ON/OFF.           */

    for(i=0; freq_band[i]!=-1 && set_para[0]!= freq_band[i]; i++);


    if (freq_band[i] == -1)
    {
        error(0);
        return 1;
    }
    data11 = p_data11[i];
    data32 = p_data32[i];
    add_fe = p_add[i];

/*     control word for setting SOLAR ATTENUATOR in the common box.*/

    for(i=0; sol_attr[i]!=-1 && set_para[1] !=sol_attr[i]; i++);
/*    if(sol_attr[i] == -1)
    {
        error(1);
        return 1;
    }
*/    data12 = p_data12[i];
    data21 = p_data21[i];

/*     control word for setting SWAP SWITCH in common box.*/

    if (set_para[2]==0)
    {
        data22=0x00;
        sprintf(SWAP,"Unswapped");
    }
    else
    {
        data22=0x02;
        sprintf(SWAP,"Swapped");
    }

/*     control word for setting NOISE CAL LEVEL in front end box.*/

    for(i=0; set_para[3]!=ns_cal[i]; i++);
    ns_cal_index=i;

    data31 = p_data31[i];

    data1=data11+data12;  /* data for the lower latch for ch1 & ch2 in common box.*/
    data2=data21+data22;  /* data for the higher latch for ch1 & ch2 in common box.*/
    data3=data31+data32;  /* data for front end box.
			 address of this box is given by add_fe.*/

/* Date Nov 7, 2016, NDS Email 25 Oct, Walsh, NG Generation, Walsh group and FE settings through MCM 4 
   -- JPK */

            for(i=0;i<32;i++) digWord[i]=0;

            for( i = 0; i < 2;i++)
            {

                   /* ---- NG Cycle ------ bit D0 D1 */
                  switch(set_para[32]) {
                       case 0  :NG_cycle = 0; break;
                       case 25 :NG_cycle = 1; break;
                       case 50 :NG_cycle = 2; break;
                       case 100:NG_cycle = 3; break;
                       default : fprintf(stderr, "\n ### WARNING : Invalid NG_cycle value, considering 0 \n"); NG_cycle = 0;
                  }
                   getbitDO((short)NG_cycle); for (b=0;b<2;b++) { digWord[b + (i*16)] = bitDO[b]; }

                   /* ---- NG Frequency -------------------- bit D2 D3 */
                   NG_Freq[i] = set_para[33];
                   if ( NG_Freq[i] > 3 || NG_Freq[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid NG_Freq value %d \n", set_para[33]);
                         exit(1);
                   }
                   getbitDO((short)NG_Freq[i]); for (b=0;b<2;b++) { digWord[ 2 + b + (i*16)] = bitDO[b]; }

                   /* ------ FE Sel ON OFF ----------- bit D4 */
                   FE_Sel[i] = set_para[34];
                   if ( FE_Sel[i] > 1 || FE_Sel[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid FE_Sel value %d \n", set_para[34]);
                         exit(1);
                   }
                   digWord[4 + (i * 16) ] = FE_Sel[i];

                   /* --------- Walsh Pattern ------- bit D5,6,7 */
                   WLSH_P[i] = set_para[35];
                   if ( WLSH_P[i] > 7 || WLSH_P[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid WLSH_P value %d \n", set_para[35]);
                         exit(1);
                   }
                   getbitDO((short)WLSH_P[i]); for (b=0;b<3;b++) { digWord[ 5 + b + (i*16)] = bitDO[b]; }

                   /* ------------ Walsh Freq --------- bit D8,9 */
                   WLSH_Freq[i] = set_para[36];
                   if ( WLSH_Freq[i] > 3 || WLSH_Freq[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid WLSH_P value %d \n", set_para[36]);
                         exit(1);
                   }
                   getbitDO((short)WLSH_Freq[i]); for (b=0;b<2;b++) { digWord[ 8 + b + (i*16)] = bitDO[b]; }

                   /* SPARE -------------------- bit D10,11 - SPARE */
                   digWord[ 10 + (i*16)] = digWord[ 11 + (i*16)] = 0;

                   /* Walsh Group ------------ bit D12,13, D14- Group  FREEZED TO VALUE 1
                   WLSH_Grp[i] = set_para[37];
                   if ( WLSH_Grp[i] > 1 || WLSH_Grp[i] < 0 ) {
                         fprintf(stderr, "\n ### Error : Invalid WLSH_P value %d \n", set_para[37]);
                         exit(1);
                   }
                   getbitDO((short)WLSH_Grp[i]); 
                   */

                   digWord[ 12 + (i*16)] = digWord[ 13 + (i*16)]= digWord[ 14 + (i*16)] = 1;

                   /* bit D15 - EN */
                    digWord[15 + (i * 16) ] = i?1:0;


#ifdef JDEBUG
                fprintf(stderr, "\n CH[%d] - NG_cycle[%d] , NG_Freq[%d], FE_sel[%d], WLSH_P[%d] WLSH_Freq[%d] ", i,
                        NG_cycle, NG_Freq[i], FE_Sel[i], WLSH_P[i], WLSH_Freq[i] );    
#endif

            }


                    mcm4word[0] = 0 ;  mcm4word[1] = 0;

                    for(i=0 ;i <  16 ;i++) {
                       if(digWord[i+16]) mcm4word[0] = mcm4word[1] | (1 << i );
                       if(digWord[i]) mcm4word[1] = mcm4word[0] | (1 << i );
                    }

#ifdef JDEBUG
                    fprintf(stderr,"\n");
                    for( i = 0 ; i < 16;i++)
                        fprintf(stderr, "%d", mcm4word[0] & ( 1 << i )? 1:0);
                    fprintf(stderr,"\n");
                    for( i = 0 ; i < 16 ;i++)
                        fprintf(stderr, "%d", mcm4word[1] & ( 1 << i )?1:0);
                    fprintf(stderr,"\n");

              fprintf(stderr,"\n* MCM4BITS :");
              fprintf(stderr, "ana 0%4xx 0%4xx\n", mcm4word[1], mcm4word[0]);   
              fprintf(stderr, "ana 0%4xx 0%4xx\n", mcm4word[1] & (~ (unsigned short)( 1 << 4)), mcm4word[0] & (~(unsigned short)( 1 << 4 )) );   
              fprintf(stderr, "ana 0%4xx 0%4xx\n", mcm4word[1] | ( 1 << 4) , mcm4word[0] | ( 1 << 4 ) );   
#endif

	    f1 = fopen("/home/observer/set/user/user1/fe.set", "w");
	    if(f1==NULL)
	    {
		printf("File opening error.\nAbnormal exit...");
		return 20;
	    }

	    fprintf(f1,"\n* run file for setting FRONT END parameters.\n");

	    fprintf(f1,"\nmpa 3 0 4 5");
	    fprintf(f1,"\ncomm 0;dest 17;t3v\n"); fprintf(f1,"\n");
	  /* fprintf(f1,"\ncomm 29;dest 17;t3v\n"); fprintf(f1,"\n"); */

                   /* bit D4 */
            for(i = 0 ; i < 2;i++)
            fprintf(f1,"* CH[%d] Settings : NG_cycle[%d], NG_Freq[%d] FE_sel[%d] WLSH_P[%d] WLSH_Freq[%d] \n", 
                             i, NG_cycle, NG_Freq[i], FE_Sel[i], WLSH_P[i],WLSH_Freq[i]);

         /* fprintf(f1,"\nana 070%02Xx 0f0%02Xx",FEDAT_OFF,FEDAT_OFF);  
	    fprintf(f1,"\n\n* MCM5_ON with Walsh:%s,Walsh Group:%s and Noise Generator duty cycle:%d\n",WE,WG,set_para[14]);
	    fprintf(f1,"\nana 070%02Xx 0f0%02Xx",FEDAT_ON,FEDAT_ON);
	    fprintf(f1,"\nst32dig(4)"); */

            fprintf(f1, "\n* MCM 5 OFF \n");
            fprintf(f1, "ana 0%4xx 0%4xx\n", mcm4word[1] & (~ (unsigned short)( 1 << 4)), mcm4word[0] & (~(unsigned short)( 1 << 4 )) );   
	    fprintf(f1, "st32dig(4)\n");

            fprintf(f1, "\n* MCM 5 ON \n");
            fprintf(f1, "ana 0%4xx 0%4xx\n", mcm4word[1] | ( 1 << 4 ), mcm4word[0] | ( 1 << 4) ); 
	    fprintf(f1, "st32dig(4)\n");



	    fprintf(f1,"\n\n* Freq. Band:%d MHz, Solar Attn.:%d dB\n",set_para[0],set_para[1]);

	    fprintf(f1,"\n* Polarisation:%s, Noise-Cal level:%s",SWAP,N_CAL[ns_cal_index]);
	    
	    fprintf(f1,"\n\nenamcmq");

   /* Order changed to test new 325 upgraded feed with Rabbit -- 23 May 2016 */
	/*   fprintf(f1,"\nana 07%02xx 00%02xx 08%02xx 00%02xx",data1, data1, data2, data2); */
	     fprintf(f1,"\nana 09%02xx 00%02xx 0A%02xx 00%02xx",data1, data1, data2, data2);   
	    fprintf(f1,"\nst64dig(5)");
	/*   fprintf(f1,"\nana 09%02xx 00%02xx 0A%02xx 00%02xx",data1, data1, data2, data2); */
	     fprintf(f1,"\nana 07%02xx 00%02xx 08%02xx 00%02xx",data1, data1, data2, data2);   
	    fprintf(f1,"\nst64dig(5)");
	    for(i=1; i<add_fe; i++)
			{
		fprintf(f1,"\nana %02x16x 0016x",i);
		    fprintf(f1,"\nst32dig(5)");
			}
	    for(i=add_fe+1; i<7; i++)
			{
		fprintf(f1,"\nana %02x16x 0016x",i);
		    fprintf(f1,"\nst32dig(5)");
			}
	    fprintf(f1,"\nana %02x%02xx 00%02xx",add_fe,data3,data3);
    fprintf(f1,"\nst32dig(5)");
    fprintf(f1,"\ndismcmq\n");

#ifdef MON
   /* FE-CB Monitoring */
      fprintf(f1,"\n\n* FE-CB MONITORING \n");
      fprintf(f1,"comm 710;dest=5;t3v;t3v\n");
      fprintf(f1,"comm 700;dest=5;CPA(1)=%d;t3v;t3v\n",add_fe-1);
#endif

            fprintf(f1, "\n* MCM ON \n");
            fprintf(f1, "ana 0%4xx 0%4xx\n", mcm4word[1] | ( 1 << 4 ), mcm4word[0] | ( 1 << 4) ); 
	    fprintf(f1, "st32dig(4)\n");

            fprintf(f1, "\n* MCM OFF \n");
            fprintf(f1, "ana 0%4xx 0%4xx\n", mcm4word[1] & (~ (unsigned short)( 1 << 4)), mcm4word[0] & (~(unsigned short)( 1 << 4 )) );   
	    fprintf(f1, "st32dig(4)\n");

/*
    fprintf(f1,"\n\n* MCM5_ON  with Walsh:%s,Walsh Group:%s and Noise Generator duty cycle:%d\n",WE,WG,set_para[14]);
    fprintf(f1,"\nana 070%02Xx 0f0%02Xx",FEDAT_ON,FEDAT_ON);
    fprintf(f1,"\nst32dig(4)\n");
    fprintf(f1,"\n\n* MCM5_OFF with Walsh:%s,Walsh Group:%s and Noise Generator duty cycle:%d\n",WE,WG,set_para[14]);
    fprintf(f1,"\nana 070%02Xx 0f0%02Xx",FEDAT_OFF,FEDAT_OFF);
    fprintf(f1,"\nst32dig(4)\n");
*/

    fprintf(f1,"\nmpa 1 5");
    fprintf(f1,"\ncomm 30;dest 17;t3v\n\n");

    fclose(f1);
    return 0;
    }

/************************************************************************/
/* generates a run file LO.SET for lo freq setting (NEW SYN1 and SYN2 ).*/

int set_newlo(void)
{
    int lofreq,vco_no,tmp1,stp,frq,bitset,tmp2,temp=0,lofreq1,lofreq2;
    char mask[5][8],mask1[5][8],mode[80],maska[2][8],mask1a[2][8];
    char permission,ans[10],maskp[2][8],mask1p[2][8];
    char masksp[6][8] = {"00080x","08080x","01000x","09000x","0600Dx","0E01Dx"};
    int mnum,nnum,mdec,ndec,bx,acum,acuma,acump,yigsc,cnt=2,maxfrq;
	int acum1, acum2, acum3;
    FILE *f1,*fa,*fp;
	int MASK1=61440;
	int MASK2=4095;


	lofreq1 = set_para[4];
	lofreq2 = set_para[5];
	maxfrq = (lofreq2 >= lofreq1) ? lofreq2 : lofreq1;

/*	if((lofreq1 >=601 && lofreq1 <=1800) ||
		(lofreq2 >=601 && lofreq2 <=1800))
	{
	printf("\nFor LO from YIG select\n");
	printf("\nAage(3) Ya Piche(1) Enter for default (0) ? : ");
	gets(ans);
	if(ans == '\0') sc = 0;
	else
	sc = atoi(ans);
	}       */

    if (lofreq1 < 100 || lofreq1 > 1795 || lofreq2 < 100 || lofreq1 > 1795)
	{
        error (4);
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
		printf("\nSingal VCO cannot support two diffrent Frequencies ");
		error(4);
		return(1);
	}

   if(((lofreq1>=355) && (((float)lofreq1/5-(int)(lofreq1/5)) != 0))
   || ((lofreq2>=355) && (((float)lofreq2/5-(int)(lofreq2/5)) != 0)))
	{
	    printf("For VCO 3 and VCO 4, lofreq only in steps of 5MHz.\n");
	    error(4);
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
	sprintf(mask[4], "06005x");
	sprintf(mask1[4],"0E015x");
        }
    if(vco_no == 2 || vco_no == 3)
        {
	sprintf(mask[4], "06002x");
	sprintf(mask1[4],"0E012x");
        }

    if((vco_no == 1) && ( lofreq1 <= lofreq2))
        {
	sprintf(mask[2], "00168x");
	sprintf(mask1[2],"08168x");
	sprintf(mask[3], "01001x");
	sprintf(mask1[3],"09001x");
        }
    if((vco_no == 1) && ( lofreq1 > lofreq2))
        {
        sprintf(mask[2], "001a8x");
        sprintf(mask1[2],"081a8x");
        sprintf(mask[3], "01001x");
        sprintf(mask1[3],"09001x");
        }
    
    if((vco_no == 2) && (lofreq1 <= lofreq2)) 
        {
	sprintf(mask[2], "00146x");
	sprintf(mask1[2],"08146x");
	sprintf(mask[3], "01002x");
	sprintf(mask1[3],"09002x");
        }
     if((vco_no == 2) && (lofreq1 > lofreq2))  
        {
        sprintf(mask[2], "00186x");
        sprintf(mask1[2],"08186x");
        sprintf(mask[3], "01002x");
        sprintf(mask1[3],"09002x");
        }
    
    if((vco_no == 3) && (lofreq1 <= lofreq2))
        {
	sprintf(mask[2], "00245x");
	sprintf(mask1[2],"08245x");
	sprintf(mask[3], "01004x");
	sprintf(mask1[3],"09004x");
        }
    if((vco_no == 3) && (lofreq1 > lofreq2)) 
        {
        sprintf(mask[2], "00285x");
        sprintf(mask1[2],"08285x");
        sprintf(mask[3], "01004x");
        sprintf(mask1[3],"09004x");
        }
   
    if((vco_no == 4) && (lofreq1 <= lofreq2)) 
        {
	sprintf(mask[2], "00258x");
	sprintf(mask1[2],"08258x");
	sprintf(mask[3], "01008x");
	sprintf(mask1[3],"09008x");
        }
  if((vco_no == 4) && (lofreq1 > lofreq2))  
        {
        sprintf(mask[2], "00298x");
        sprintf(mask1[2],"08298x");
        sprintf(mask[3], "01008x");
        sprintf(mask1[3],"09008x");
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
        if(vco_no == 4 ){
            acum = 4095 - acum;
                sprintf(mask[0], "04%03Xx",acum);
                sprintf(mask1[0],"0C%03Xx",acum);
        }
        else {
                acum1 = acum & MASK1;
        acum2 = acum ^ MASK2;
        acum3 = acum1 | acum2;
        /*printf("04%03Xx 04%03Xx\n \n", acum, acum3);*/
        sprintf(mask[0], "04%03Xx",acum3);
        sprintf(mask1[0],"0C%03Xx",acum3);
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

    sprintf(mask[1], "02%03Xx",acum);
    sprintf(mask1[1],"0A%03Xx",acum);
    if(vco_no == 4)
    {
    sprintf(maska[1], "02%03Xx",acuma);
    sprintf(mask1a[1],"0A%03Xx",acuma);
    sprintf(maskp[1], "02%03Xx",acump);
    sprintf(mask1p[1],"0A%03Xx",acump);
    }


    if(cnt == 1)
    {
    f1 = fopen("/home/observer/set/user/user1/lo.set","w");
    if (f1 == NULL)
        {
        printf("File opening error.\nAbnormal exit...");
        return 20;
	}
    if(maxfrq > 600)
    {
    fa = fopen("/home/observer/set/user/user1/loa.set","w");
    if (f1 == NULL)
        {
        printf("File opening error.\nAbnormal exit...");
        return 20;
	}
    fp = fopen("/home/observer/set/user/user1/lop.set","w");
    if (f1 == NULL)
        {
        printf("File opening error.\nAbnormal exit...");
        return 20;
	}
    }

    fprintf(f1,"\n* Run file for setting SYN1 LO at %d MHz. and SYN2 LO at %d MHz", set_para[4],set_para[5]);
    fprintf(f1,"\n* MODE : %s\n",mode);
    if(maxfrq > 600)
    {
    fprintf(fa,"\n* Run file for incrementing one bit of YIGDAC for LO at %d MHz.", maxfrq);
    fprintf(fp,"\n* Run file for decrementing one bit of YIGDAC for LO at %d MHz.", maxfrq);
    }
  }/* if cnt == 1 */

    if(cnt == 1)
    {
    fprintf(f1,"\n mpa 2 2 3");
    fprintf(f1,"\n comm 29;dest 17;t3v");
/*    fprintf(f1,"\n enamcmq\n"); */
    if(maxfrq > 600)
    {
    fprintf(fa,"\n mpa 2 2 3");
    fprintf(fa,"\n comm 29;dest 17;t3v");
    fprintf(fa,"\n enamcmq\n");
    fprintf(fp,"\n mpa 2 2 3");
    fprintf(fp,"\n comm 29;dest 17;t3v");
    fprintf(fp,"\n enamcmq\n");
    }
    }

    if((lofreq1 != lofreq2) || (cnt == 1))
    {
    fprintf(f1,"\n* FREQ : %d MHz, STP : %d MHz, VCO : %d",lofreq,stp,vco_no);
    if(maxfrq > 600 && vco_no == 4)
    {
    fprintf(fa,"\n* FREQ : %d MHz, STP : %d MHz, VCO : %d   (AAGE) ",lofreq,stp,vco_no);
    fprintf(fp,"\n* FREQ : %d MHz, STP : %d MHz, VCO : %d   (PICHE)",lofreq,stp,vco_no);
    }
    }
    if((lofreq1 == lofreq2) && (cnt == 1))
       {
        fprintf(f1,"\n enamcmq\n"); 
	fprintf(f1,"\n ana %s %s %s %s",masksp[0],masksp[1],masksp[2],masksp[3]);
	if(vco_no == 4) fprintf(f1,"\n st64dig(2)");
	else fprintf(f1,"\n st64dig(3)");

	fprintf(f1,"\n ana %s %s",masksp[4],masksp[5]);
	if(vco_no == 4) fprintf(f1,"\n st32dig(2)");
	else fprintf(f1,"\n st32dig(3)");
        fprintf(f1,"\n dismcmq\n"); 
	}
    else
    {
        fprintf(f1,"\n enamcmq\n"); 
    fprintf(f1,"\n ana %s %s",masksp[2],masksp[3]);
    if(vco_no == 4) fprintf(f1,"\n st32dig(3)");
    else fprintf(f1,"\n st32dig(2)");

    fprintf(f1,"\n ana %s %s %s %s", mask[0], mask1[0], mask[1], mask1[1]);
    if(vco_no == 4) fprintf(f1,"\n st64dig(3)");
    else fprintf(f1,"\n st64dig(2)");

    /******/
    if(maxfrq > 600 && vco_no == 4)
    {
    fprintf(fa,"\n ana %s %s", maska[1], mask1a[1]);
    fprintf(fa,"\n st32dig(3)");
    fprintf(fp,"\n ana %s %s", maskp[1], mask1p[1]);
    fprintf(fp,"\n st32dig(3)");
    }
   /******/

    fprintf(f1,"\n ana %s %s %s %s", mask[2], mask1[2], mask[3], mask1[3]);
    if(vco_no == 4) fprintf(f1,"\n st64dig(3)");
    else fprintf(f1,"\n st64dig(2)");

    fprintf(f1,"\n ana %s %s", mask[4], mask1[4]);
    if(vco_no == 4) fprintf(f1,"\n st32dig(3)");
    else fprintf(f1,"\n st32dig(2)");
        fprintf(f1,"\n dismcmq\n"); 
    }

    if(cnt == 1) lofreq = lofreq2;

}

    /* fprintf(f1,"\n\n dismcmq\n"); */
    if(maxfrq > 600)
    {
    fprintf(fa,"\n\n dismcmq\n");
    fprintf(fp,"\n\n dismcmq\n");
    fclose(fa);
    fclose(fp);
    }
    fprintf(f1," enalo1mon; enalo2mon\n");
    fclose(f1);
    return 0;
    }
/************************************************************/
/* generates a run file IF.SET for if freq setting. */

int NewIF()
{
        int i,j,bw,lsb1,lsb2,xgain_ch1,xgain_ch2,fbv_dec1,fbv_dec2,alc=0;
        int if_bw2[] = {0xc8, 0xd0, 0xe0};
        int if_bw1[] = {0x01, 0x02, 0x04};
        int ADDR=10; /* MCM adadr*/
        int pratr1,pratr2, bw1, bw2, alc1, alc2;
        float gain_ch1=post_gain[0], gain_ch2=post_gain[1];
        FILE *f1;
        pratr1=set_para[6]; pratr2=set_para[8]; bw1=set_para[10]; bw2=set_para[11]; alc1=set_para[12]; alc2=set_para[13];
//printf("\nif_bw1[0] = %x\nif_bw1[1] = %x\nif_bw1[2] = %x\n",if_bw1[0],if_bw1[1],if_bw1[2]);

/*========================Error Checking of Different Parameters Entered START============================*/

        if( (pratr1%2 !=0) || (pratr2%2 !=0) || (pratr1<0 || pratr2<0 || pratr1>30 || pratr2>30) )
        {
        printf("Improper Attenuation in setup.txt\nENTER PROPER ATTENUATIONS\nRange :- 0 to 30 in multiples of 2\n");
        exit(1);
        }
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

//      alc = alc1 | alc2<<1;
//printf("\nalc1 = %x\talc2 = %x\n",alc1,alc2);
        if(bw1==6) i=0; else if(bw1==16) i=1; else if(bw1==32) i=2; else { printf("Enter Proper Bandwidth (6,16,32 MHz) for CH-1\n"); exit(1);}
        if(bw2==6) j=0; else if(bw2==16) j=1; else if(bw2==32) j=2; else { printf("Enter Proper Bandwidth (6,16,32 MHz) for CH-2\n"); exit(1);}

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
        alc = alc1 | alc2<<1;

/*========================Error Checking of Different Parameters Entered END============================*/

        f1 = fopen("/home/observer/set/user/user1/if.set","w+t");
        fprintf(f1,"\n* Run file for setting IF parameters.\n");
        fprintf(f1, "\nmpa 1 %d\naddmcm\n\nenamcmq\n",ADDR);
//PRATR :-
        fprintf(f1, "* PRE ATTENUATION CH-1 -> %d\tCH-2 -> %d \n",pratr1, pratr2);
	fprintf(f1, "ana 0B3%c%cx 0F3%c%cx 0B3%c%cx 0F3%c%cx\n", PratrCode(pratr2), PratrCode(pratr1), PratrCode(pratr2), PratrCode(pratr1), PratrCode(pratr2), PratrCode(pratr1), PratrCode(pratr2), PratrCode(pratr1)); 
        fprintf(f1, "\nst64dig(%d)\n\n",ADDR);
        fprintf(f1,"* Post Gain CH-1 -> %8.3f dB\tCH-2 -> %8.3f dB \n", gain_ch1, gain_ch2);
//Gain :-
        xgain_ch1 = 90 - (2*gain_ch1);
        xgain_ch2 = 90 - (2*gain_ch2);
        lsb1 = xgain_ch1 & 0x7;
        lsb2 = xgain_ch2 & 0x7;
        lsb1 = ((lsb2<<5) | (lsb1<<2));
        lsb1 = lsb1 | alc;
//printf("\nlsb1 = %x and alc = %x\n",lsb1,alc);
	fprintf(f1, "ana 0B7%x%xx 0F7%x%xx 0BF%x%xx 0FF%x%xx\n",xgain_ch2>>3&0xf, xgain_ch1>>3&0xf, xgain_ch2>>3&0xf, xgain_ch1>>3&0xf, (lsb1>>4)&0xf,lsb1&0xf,(lsb1>>4)&0xf, lsb1&0xf );
        fprintf(f1, "\nst64dig(%d)\n",ADDR);
//IFBW :-
        bw = if_bw1[i]+if_bw2[j];

        fprintf(f1,"\n* BW in CH1 & CH2 are, %d & %d resp.\n", bw1, bw2);
        fprintf(f1,"\nana 0FB%02xx 0FB%02xx 0BB%02xx 0FB%02xx\nst64dig(%d)\n\ndismcmq",bw,bw,bw,bw,ADDR);
/*      printf("\nSUCESSFULLY CREATED IF.SET FILE\n");*/
        return(0);

}

char PratrCode(int g)
{
        switch (g)
        {
             case 0  :{return '4';}
             case 2  :{return '5';}
             case 4  :{return '6';}
             case 6  :{return '7';}
             case 8  :{return '0';}
             case 10 :{return '1';}
             case 12 :{return '2';}
             case 14 :{return '3';}
             case 16 :{return 'C';}
             case 18 :{return 'D';}
             case 20 :{return 'E';}
             case 22 :{return 'F';}
             case 24 :{return '8';}
	     case 26 :{return '9';}
             case 28 :{return 'A';}
             case 30 :{return 'B';}
             default :{printf("Enter proper attenuation value:");exit(1); }
        }
}
/*-----------------------------------------------------------------*/

    int main (void)
    {
    FILE *f1;
    char SetLine[LINE_LEN];
    int i;
    
    f1=fopen("/home/observer/set/user/user1/setupnew.txt","r");
    if(f1==NULL)
        {
	printf("\nCan not open 'setupnew.txt'. Abnormal exit...\n");
        return 20;
        }
    i=0;
    while(!feof(f1))
        {
        fgets(SetLine, LINE_LEN-1, f1);
        filter_comment(SetLine);
        if(is_empty(SetLine))
            continue;
	if(i==7)
                post_gain[0]=atof(SetLine);
        if(i==9)
                post_gain[1]=atof(SetLine);
	if(i==18)
                gab_attn[0]=atof(SetLine);
        if(i==19)
                gab_attn[1]=atof(SetLine);
       else 	
	set_para[i]=atoi(SetLine);
        i++;
        }

    if(set_fe())
        printf("Error in creating 'fe.set'...\n");
        else
        printf("Successfully created 'fe.set'.\n");
    if(set_newlo())
        printf("Error in creating 'lo.set'...\n");
        else
        printf("Successfully created 'lo.set'.\n");
    if(NewIF())
        printf("Error in creating 'if.set'...\n");
        else
        printf("Successfully created 'if.set'.\n");
    if(set_gab())
        printf("Error in creating 'gab.set'...\n");
        else
        printf("Successfully created 'gab.set'.\n");

    return 0;
    }

/*-----------------------------------------------------------------*/

// C program to implement one side of FIFO 
// This side writes first, then reads 
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main() 
{ 
    int fd; 
  
    // FIFO file path 
    char * myfifo = "/tmp/myfifo"; 

    unsigned char buff[18];
       buff[0]=0;
       buff[1]=0x11;
       buff[2]=0;
       buff[3]=0;
       buff[4]=1;
       buff[5]=0;
       buff[6]=9;
       buff[7]=0;
       buff[8]=4;
       buff[9]=0xff;
       buff[10]=0xff;
       buff[11]=0xff;
       buff[12]=0xff;
       buff[13]=0xff;
       buff[14]=0xff;
       buff[15]=0xff;
       buff[16]=0xff;
       buff[17]=0;

  
    // Creating the named file(FIFO) 
    // mkfifo(<pathname>, <permission>) 
    mkfifo(myfifo, 0666); 
  
    char arr1[80], arr2[80]; 
    while (1) 
    { 
        // Open FIFO for write only 
        fd = open(myfifo, O_WRONLY); 
  
        // Take an input arr2ing from user. 
        // 80 is maximum length 
      //  fgets(arr2, 80, stdin); 
  
        // Write the input arr2ing on FIFO 
        // and close it 
	int i,length;
         length = buff[1] + (buff[2]<<8); 
         write(fd, buff, length); 
	/* for (i=0; i<(length+1); i++)
          printf("serial PKT in USB_RS485 ====>0x%02X\n",(unsigned char) buff[i]);
	 */
     	memset(buff,'\0',sizeof(buff)); 
     //       break;
       //  close(fd); 
  
        // Open FIFO for Read only 
       /*  fd = open(myfifo, O_RDONLY); 
  
        // Read from FIFO 
        read(fd, arr1, sizeof(arr1)); 
  
        // Print the read message 
        printf("User2: %s\n", arr1); 
        close(fd); */
    } 
    return 0; 
} 

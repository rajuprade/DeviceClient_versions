INCLUDEDIR=-L. -lftd2xx -Wl,-rpath /usr/local/lib 
CFLAGS1 = -Wall -DDEBUG -g ${INCLUDEDIR} -I. 
CFLAGS = -g -I.
LIBS = -lm

deviceClient: deviceClient.o wrapperCom.o fecb_eth.o fecb_mcm.o low_level_mcm.o usb_rs485.o response.o 
	gcc $(CFLAGS) -o deviceClient deviceClient.o wrapperCom.o fecb_eth.o fecb_mcm.o low_level_mcm.o usb_rs485.o response.o  $(LIBS) $(CFLAGS1)      


deviceClient.o: wrapperCom.o fecb_eth.o fecb_mcm.o response.o low_level_mcm.o usb_rs485.o
	gcc $(CFLAGS) -c deviceClient.c 

wrapperCom.o: wrapperCom.c wrapperCom.h
	gcc $(CFLAGS) -c wrapperCom.c 

fecb_eth.o: fecb_eth.c fecb_eth.h
	gcc $(CFLAGS) -c fecb_eth.c 

fecb_mcm.o: fecb_mcm.c fecb_mcm.h
	gcc $(CFLAGS) -c fecb_mcm.c

low_level_mcm.o: low_level_mcm.c low_level_mcm.h
	gcc $(CFLAGS) -c low_level_mcm.c 

usb_rs485.o: usb_rs485.c usb_rs485.h
	gcc -c usb_rs485.c 

response.o: response.c response.h
	gcc $(CFLAGS) -c response.c                     
clean: 
	rm -f  *.o deviceClient    

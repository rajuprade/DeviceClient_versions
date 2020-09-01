INCLUDEDIR=-L. -lftd2xx -Wl,-rpath /usr/local/lib 
CFLAGS1 = -Wall -DDEBUG -g ${INCLUDEDIR} -I. 
CFLAGS = -g -I.
LIBS = -lm

deviceClient: deviceClient.o wrapperCom.o fps_eth.o fps_mcm.o fps_cmd_ps.o usb_rs485.o 
	gcc $(CFLAGS) -o deviceClient deviceClient.o wrapperCom.o fps_eth.o fps_mcm.o fps_cmd_ps.o usb_rs485.o $(LIBS) $(CFLAGS1)      


deviceClient.o: wrapperCom.o fps_eth.o fps_mcm.o fps_cmd_ps.o usb_rs485.o
	gcc $(CFLAGS) -c deviceClient.c 

wrapperCom.o: wrapperCom.c wrapperCom.h
	gcc $(CFLAGS) -c wrapperCom.c 

fps_eth.o: fps_eth.c fps_eth.h
	gcc $(CFLAGS) -c fps_eth.c 

fps_mcm.o: fps_mcm.c define.h
	gcc $(CFLAGS) -c fps_mcm.c

fps_cmd_ps.o: fps_cmd_ps.c define.h
	gcc $(CFLAGS) -c fps_cmd_ps.c 

usb_rs485.o: usb_rs485.c usb_rs485.h
	gcc -c usb_rs485.c                      
clean: 
	rm -f  *.o deviceClient    

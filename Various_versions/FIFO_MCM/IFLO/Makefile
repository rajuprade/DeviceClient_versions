INCLUDEDIR=-L. -lftd2xx -Wl,-rpath /usr/local/lib 
CFLAGS1 = -Wall -DDEBUG -g ${INCLUDEDIR} -I. 
CFLAGS = -g -I.
LIBS = -lm

deviceClient: deviceClient.o wrapperCom.o iflo_eth.o iflo_mcm.o low_level_mcm.o response.o 
	gcc $(CFLAGS) -o deviceClient deviceClient.o wrapperCom.o iflo_eth.o iflo_mcm.o low_level_mcm.o response.o  $(LIBS) $(CFLAGS1)      


deviceClient.o: wrapperCom.o iflo_eth.o iflo_mcm.o response.o low_level_mcm.o 
	gcc $(CFLAGS) -c deviceClient.c 

wrapperCom.o: wrapperCom.c wrapperCom.h
	gcc $(CFLAGS) -c wrapperCom.c 

iflo_eth.o: iflo_eth.c iflo_eth.h
	gcc $(CFLAGS) -c iflo_eth.c 

iflo_mcm.o: iflo_mcm.c iflo_mcm.h
	gcc $(CFLAGS) -c iflo_mcm.c

low_level_mcm.o: low_level_mcm.c low_level_mcm.h
	gcc $(CFLAGS) -c low_level_mcm.c 

response.o: response.c response.h
	gcc $(CFLAGS) -c response.c                     
clean: 
	rm -f  *.o deviceClient    

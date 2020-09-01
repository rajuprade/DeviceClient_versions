########################################################################
# CMS Wrapper Server $ Version 1.0, Apr 30,2011, SRB $
# use Make -f Makefile
# This makefile is used to build targets
# invokeWrapper
#modified for Libxml inclusion.............
########################################################################
.SUFFIXES:
.SUFFIXES:	.c .o
INCLUDEDIR	=-I /usr/include/libxml2 
HEADER		=invokeWrapper.h wrapperCom.h handleQue.h xmlParse.h xmlResp.h servCMS.h consumCMS.h devSpec.h devServo.h
HEADER1		= wrapperCom.h   
CC              =cc -g -fPIC -U_DATASERVER_
F77=gfortran -fno-second-underscore -fPIC -g
CFLAGS          = -Wall -g ${INCLUDEDIR} -I.
LIBS= -lm -L/usr/lib -lnsl
LIBXML=-L/usr/include/libxml2 -lxml2
LIBTHREAD=-lpthread
LIBASTRO=-L. -lcalcepochc -lsla
LIBS0=${LIBS} ${LIBXML} ${LIBTHREAD} ${LIBASTRO}
LIBS1=${LIBS} 
LINK= ${F77}
#LINK = ${CC} -g 

CFILES0		= invokeWrapper.c wrapperCom.c handleQue.c xmlParse.c xmlResp.c servCMS.c consumCMS.c \
		devSentinal.c devFrontend.c devDataserver.c devSigcon.c devBackend.c devServo.c trkinterface.c
CFILES1		= deviceClient.c wrapperCom.c

OBJECT0			= ${CFILES0:.c=.o}
OBJECT1			= ${CFILES1:.c=.o}

TARGET		= invokeWrapper deviceClient
####################################################
all:	${HEADERS} ${TARGET}

.c.o:
	${CC} -c ${CFLAGS} -o ${<:.c=.o} $<

invokeWrapper:	${HEADER} ${OBJECT0}
		${LINK} ${OBJECT0} ${LIBS0} -o $@ 

deviceClient: ${HEADER1} ${OBJECT1}
		${LINK} ${OBJECT1} ${LIBS1} -o $@

clean:
	/bin/rm -f ${OBJECT0} ${OBJECT1} ${TARGET}


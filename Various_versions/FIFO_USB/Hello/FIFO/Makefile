TOPDIR  := $(shell cd ..; pwd)
include $(TOPDIR)/Rules.make

APP = usbrs485

all: $(APP)

$(APP): usbrs485.c
	$(CC) -o $(APP) usbrs485.c $(CFLAGS)     
        
clean:
        rm -f *.o ; rm $(APP)


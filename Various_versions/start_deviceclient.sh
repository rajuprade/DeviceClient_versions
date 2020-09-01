#!  /bin/bash

cd /home/raj/Desktop/FIFO_MCM_28JAN/MCM

sudo ./mcmtest >> /dev/null 2>&1 &

if [ $? -eq 0 ]
then
  echo "USB BASED MCMTEST PROGRAM ${HOSTNAME} started successfully"
else
  echo "### ERROR USB BASED MCMTEST PROGRAM not started"

fi

cd /home/raj/Desktop/FIFO_MCM_28JAN/fecb

#./stop_client.sh
./deviceClient >> /dev/null 2>&1 &

if [ $? -eq 0 ]
then
  echo "FIFO BASED FECB Client ${HOSTNAME} started successfully"
else
  echo "### ERROR FIFO BASED FECB Client not started"

fi

cd /home/raj/Desktop/FIFO_MCM_28JAN/FPS_MCM

#./stop_client.sh
./deviceClient >> /dev/null 2>&1 &

if [ $? -eq 0 ]
then
  echo "FIFO BASED FPS Client ${HOSTNAME} started successfully"
else
  echo "### ERROR FIFO BASED FPS Client not started"

fi



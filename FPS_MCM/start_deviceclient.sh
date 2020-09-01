#!  /bin/bash

cd /home/lmcuser/FPS_MCM

./deviceClient >> /dev/null 2>&1 &

if [ $? -eq 0 ]
then
  echo "FPS Client ${HOSTNAME} started successfully"
else
  echo "### ERROR FPS Client not started"

fi




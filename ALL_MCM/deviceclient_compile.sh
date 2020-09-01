#!  /bin/bash

cd /home/lmcuser/ALL_MCM/fecb

make clean

make

if [ $? -eq 0 ]
then
  echo "FIFO BASED FECB Client ${HOSTNAME} Compiled successfully"
else
  echo "### ERROR FIFO BASED FECB Client not compiled"

fi


cd /home/lmcuser/ALL_MCM/IFLO

make clean

make

if [ $? -eq 0 ]
then
  echo "FIFO BASED IFLO Client ${HOSTNAME} Compiled successfully"
else
  echo "### ERROR FIFO BASED IFLO Client not compiled"

fi

cd /home/lmcuser/ALL_MCM/FPS_MCM

make clean

make 

if [ $? -eq 0 ]
then
  echo "FIFO BASED FPS Client ${HOSTNAME} Compiled successfully"
else
  echo "### ERROR FIFO BASED FPS Client not Compiled"

fi


cd /home/lmcuser/ALL_MCM/MCM

make clean

make

if [ $? -eq 0 ]
then
  echo "USB BASED MCMTEST PROGRAM ${HOSTNAME} compiled successfully"
else
  echo "### ERROR USB BASED MCMTEST PROGRAM not Compiled"
fi


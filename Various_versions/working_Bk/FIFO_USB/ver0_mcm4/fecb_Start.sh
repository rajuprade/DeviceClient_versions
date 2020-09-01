#!  /bin/bash

ant=$1
for var in $ant
do
ssh $var "/home/lmcuser/fecb.sh"
done

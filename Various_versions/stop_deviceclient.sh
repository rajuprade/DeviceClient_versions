#!/bin/sh

id=`ps aux | grep deviceClient |  grep -v grep | awk '{ print $2 }'`;
if [ "${id}" != "" ]
then
kill -9 $id
id=
fi

id=`ps aux | grep mcmtest |  grep -v grep | awk '{ print $2 }'`;
if [ "${id}" != "" ]
then
sudo kill -9 $id
id=
fi

sudo rm /tmp/fecmd_fifo
sudo rm /tmp/fpscmd_fifo
sudo rm /tmp/resp_fecb
sudo rm /tmp/resp_fps

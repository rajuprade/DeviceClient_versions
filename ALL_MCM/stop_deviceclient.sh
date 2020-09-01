#!/bin/sh


id=`ps aux | grep deviceClientfecb |  grep -v grep | awk '{ print $2 }'`;
if [ "${id}" != "" ]
then
kill -9 $id
id=
fi

id=`ps aux | grep deviceClientfps |  grep -v grep | awk '{ print $2 }'`;
if [ "${id}" != "" ]
then
kill -9 $id
id=
fi

id=`ps aux | grep deviceClientiflo |  grep -v grep | awk '{ print $2 }'`;
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


sudo \rm -rf /tmp/resp_fps
sudo \rm -rf /tmp/resp_fecb
sudo \rm -rf /tmp/fpscmd_fifo
sudo \rm -rf /tmp/fecmd_fifo
sudo \rm -rf /tmp/iflocmd_fifo
sudo \rm -rf /tmp/resp_iflo

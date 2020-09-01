#!/bin/bash
 sshpass -p obs@gmrt ssh observer@shivneri
 /home/observer/gabmon -l > /tmp/iflab/test.txt
sshpass -p abc123
scp test.txt correlator@192.168.5.43:Desktop/temp.txt
#scp 12.sh correlator@192.168.5.43:/home/correlator/Desktop
convert temp.txt temp.pdf

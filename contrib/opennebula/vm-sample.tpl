NAME   = myttyserver
CPU    = 0.1
MEMORY = 64
DISK   = [
  source   = "/home/srv/cloud/images/ttylinux.0.4.img",
  target   = "hda",
  readonly = "no" ]
NIC    = [ NETWORK = "Red Virtual" ]

FEATURES=[ acpi="no" ]
RAW = [ type = "kvm", data = " <serial type='pty'> <source path='/dev/pts/3'/> <target port='1'/> </serial>" ]

# Context example
CONTEXT = [
  hostname = "$NAME",
	ip_public = "$NIC[IP, NETWORK=\"Red Virtual\"]",
	files = "/home/oneadmin/one-templates/ttylinux/init.sh /home/oneadmin/one-templates/ttylinux/id_dsa.pub",
  target = "hdc",
	root_pubkey = "id_dsa.pub",
	username = "oneadmin",
	user_pubkey = "id_dsa.pub"
]

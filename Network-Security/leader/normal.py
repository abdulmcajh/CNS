import socket
import threading
from Crypto.Hash import HMAC
from Crypto.PublicKey import RSA
import sys
import signal
import os
import select

part_pri_key=''
def auth(secret,msg):
	h = HMAC.new(secret)
	h.update(msg)
	tag=h.hexdigest()
	return tag
	
global_cl=0

def send_to_other(cl):
	while True:
		p=raw_input()
		cl.send(p)
		

def recv_from_other(cl):
	while True:
		text=cl.recv(4096)
		print "----Received",text


def authentication(glport,buf,pport):

	#connect to group leader
	global global_cl
	cl= socket.socket()      
	host = socket.gethostname()               
	cl.connect((host, int(glport)))
	
	print "connected"
	cl.send(buf)
	#send auth msg to leader
	print "dfg"
	dummy=cl.recv(4096)
	print "auth started"
	secret='sworfish'
	msg='hello'
	tag=auth(secret,msg)
	authmsg=msg+"|"+tag
	cl.send(authmsg)

	print "auth "
	#receive reply from leader
	buf = cl.recv(4096)
	print buf
	
	if buf=='Request accepted':
		#auth completed
		# communication  started .no encryption inside group
		kl= socket.socket()      
		host = socket.gethostname()               
		kl.connect((host, pport))
		#global_cl=kl
		#global_cl.send("dumdum")
		print "gininin"
		threading.Thread(name='givekey',target=givekey,args=(kl,)).start()
		threading.Thread(name='send_to_other',target=send_to_other,args=(cl,)).start()
		threading.Thread(name='recv_from_other',target=recv_from_other,args=(cl,)).start()
	else:
		print buf
	

	
global_s=0

def prikeydist(signum, frame):
	global part_pri_key
	part_pri_key=global_s.recv(8096)      
	print "received by part",part_pri_key
  

def givekey(kl):
	
	#global_cl.recv(4096)
	while 1:
		r,w,e=select.select([],[kl],[],0)
		#self.pow[i].send("dummy")
		if w :
			#print kl.recv(4096)
			kl.send(part_pri_key)
			print kl.recv(4096)
			print "sent my key to leader"
			
	

if __name__=="__main__":
	global global_s
	port=int(sys.argv[1])
	group=sys.argv[2]

	signal.signal(signal.SIGUSR2,prikeydist)

	#connect to the server
	s = socket.socket()         
	host = socket.gethostname()       
	#s.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF,10001)   
	#s.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF,10001)
	s.setblocking(1)
	s.connect((host, port))
	

	global_s=s
	print "connected to server"

	#send your pid and group
	pid=os.getpid()
	print "PID",pid
	buf=str(pid)+"|"+group
	s.send(buf)
	print "sent requst"
	#receive group leader port

	glport=s.recv(4096)
	s.send("dummmy")
	print "val received", glport
	#glport=int(glport)
	print "received port of group leader",glport
	global_s=s
	if group=='A':
		pport=53535
	else:
		pport=53536
	t1=threading.Thread(name='authentication',target=authentication,args=(glport,buf,pport,))
	t1.start()
	t1.join()
import socket
import threading
from Crypto.Hash import HMAC
from Crypto.PublicKey import RSA
import sys
import signal
import os
import select

msg_buf=[]
class mymem(object):
	
	def __init__(self):
		#incoming client data-connections
		self.mem=[]
		#incoming client connections for gathering private key
		self.pow=[]
		self.pid=[] #not needed
		#text representation of this  group  leaders public key
		self.pub_key_mine='';
		#text representation of other  group  leaders public key
		self.pub_key_other='';
		#text representation of this group private key collected from members
		self.tot_pri_key=''
		#group leader service port for clients
		self.myport=0
		#other group leader service port for clients
		self.otherport=0
		#RSA object representation of this  group  leaders public key
		self.PUK_mine=0
		#RSA object representation of other  group  leaders public key
		self.PUK_other=0
		#RSA object representation of this  group  private key
		self.PRK=0

	def collect_pri_key(self):
		"""Collect private key parts from all clients and form total private key
		"""
		temp=''
		print "hi",self.pow
		for  i in range(0,len(self.pow)):
			#self.pow[i].send("dummy")
			temp=temp+self.pow[i].recv(4096)
			self.pow[i].send("dummy")
			print "part received",temp
		self.tot_pri_key=temp
		print  "key",self.tot_pri_key
		return self.tot_pri_key
	
	

def sender(l,G):
	"""Receive messages from a clients.Encrypt each message using other group public key
	and send to other group leader
	"""
	while 1:
		if len(G.mem)>0:
			
			r,w,e=select.select(G.mem,[],[],0)
			for x in r:
				print "something sending"
				for i in range(0,len(G.mem)):
					if x is G.mem[i]:
						plain=x.recv(4096);
						print "sending ",plain
						l.send(G.PUK_other.encrypt(plain,125)[0])
						#l.send(plain)

def bufferd(l,G):
	"""Group leader receives a message (cipher). he decrypts it by collecting private keys from all other members
	Stores it in a buffer(msg_buf).
	------no mutex used on shared msg_buf
	"""
	global msg_buf
	while 1:
		r, _, _ = select.select([l], [], [],0)
		if r:
			cipher=l.recv(4096)
			print "received cipher",cipher
			#G.collect_pri_key()
			G.PRK=RSA.importKey(G.collect_pri_key())
			plain=G.PRK.decrypt(cipher)
			#plain=cipher
			print "plain ",plain
			msg_buf.append(plain)

def receiver(l,G):
	"""If client is read to receive message.Remove a message from head of message buffer.
	Send it to the client
	"""
	global msg_buf
	while 1:
		if len(G.mem)>0:
			#print "recv selecting"
			r,w,e=select.select([],G.mem,[],0)
			for x in w:
				for i in range(0,len(G.mem)):
					if x is G.mem[i] and len(msg_buf)>0:
						plain=msg_buf[0]
						msg_buf=msg_buf[1:]
						print "--received ",plain
					    #G.mem[i].send(plain)



def auth(secret,msg):
	"""Calculate Auth tag( HMAC(k,M) ) for msg(M) using secret(K)

	"""
	h = HMAC.new(secret)
	h.update(msg)
	tag=h.hexdigest()
	return tag
	

                   
def authenticator(c,ks,addr,G,pid):
	
	header=str(addr)+" : "
	print "connection  received from ",addr
	
	#after accepting a connection receive an auth msg
	buf = c.recv(4096) 
	 
	print header,"request received"
	#separate received message into msg and its auth tag
	msg,rtag= buf.split('|')
	print header,msg,rtag
	secret='sworfish'
	#find auth tag of received msg
	tag=auth(secret,msg)
	print header,tag

	#if equal accept connection
	if rtag==tag :

		c.send("Request accepted")
		#get a connection for receiving private part from client
		kl,addr=ks.accept()
		#print kl.recv(4096)
		G.mem.append(c)
		G.pid.append(pid)
		G.pow.append(kl)
		
	
	#else reject connection
	else:
		c.send("Request not authenticated")
		
	

if __name__=="__main__":
	
	#get port number from cmd

	sport=int(sys.argv[1])
	name=sys.argv[2]
	print os.getpid()
	G=mymem()
	#connect to server as group leader
	s=socket.socket()
	host = socket.gethostname() 
	s.connect((host, sport))

	print "connected to server"
	#receive public keys of groups from server
	s.send(name)
	header=s.recv(4096)
	G.pub_key_mine,G.pub_key_other,G.myport,G.otherport=header.split('|')
	G.PUK_mine=RSA.importKey(G.pub_key_mine)
	G.PUK_other=RSA.importKey(G.pub_key_other)

	G.myport = int(G.myport )
	G.otherport = int(G.otherport)
	print "myport",G.myport
	print "otherport",G.otherport
	
	#print "hi"
	#Act as server for group member requests
	gs = socket.socket()         
	host = socket.gethostname() 
	gs.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	gs.setblocking(1)
	gs.bind((host, G.myport))      
	gs.listen(5)                 
	print "serving a server on ",G.myport
	
	#B connects to A using socket gls
	if name=='B':
		#open a socket(ks) for receiving private keys
		ks = socket.socket()         
		host = socket.gethostname() 
		ks.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		ks.bind((host,53536))       
		ks.listen(5)
		print "waiting for connection from other leader"
		gls = socket.socket()      
		host = socket.gethostname()
		gls.connect((host, G.otherport))
		print "connected to other leader C",G.otherport
		threading.Thread(name='bufferd',target=bufferd,args=(gls,G,)).start()
		threading.Thread(name='sender',target=sender,args=(gls,G,)).start()
		threading.Thread(name='receiver',target=receiver,args=(gls,G,)).start()
	
	
	
	#form a connection between group leaders on sockets gs
	#A accepts a connection from B
	if name=='A':
		#open a socket(ks) for receiving private keys
		ks = socket.socket()         # Create a socket object
		host = socket.gethostname() # Get local machine name
		ks.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		ks.bind((host,53535))        # Bind to the port
		ks.listen(5)
		print "waiting for connection from other leader"
		gls,addr = gs.accept()
		print "connected to other leader",addr
		threading.Thread(name='bufferd',target=bufferd,args=(gls,G,)).start()
		threading.Thread(name='sender',target=sender,args=(gls,G,)).start()
		threading.Thread(name='receiver',target=receiver,args=(gls,G,)).start()

	while True:
		#wait for connections
		print "waiting for members"
		c, addr = gs.accept() 
		print "connected someone"
		buf=c.recv(4096) 
		print buf
		pid,group= buf.split('|')
		c.send("dummy")
		t1=threading.Thread(name='authenticator',target=authenticator,args=(c,ks,addr,G,int(pid),))
		t1.start()
		t1.join()
		#while 1:
		#	pass
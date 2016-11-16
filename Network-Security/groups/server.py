import socket
import threading
from Crypto.Hash import HMAC
from Crypto.PublicKey import RSA
import sys

class User(object):
	def __init__(self):
		self.key=[]
		#generate 2 keys for 2 groups
		self.key.append(RSA.generate(1024))
		self.key.append(RSA.generate(1024))
		
		

def auth(secret,msg):
	h = HMAC.new(secret)
	h.update(msg)
	tag=h.hexdigest()
	return tag

def sender(s,recv_pubkey,mygroup):
	while True:
		p=raw_input()
		
		cipher=recv_pubkey.encrypt(p,123)
		cipher=cipher[0]+'|||'+str(0)
		#print cipher
		s.send(cipher)
		if p=='quit':
			break
		

def receiver(s,mykey):
	while True:
		text=s.recv(4096)
		#print text
		cipher,keyid=text.split('|||')
		keyid=int(keyid)
		plain=G.key[keyid].decrypt(cipher)
		print "----Received on ",keyid," : ",plain
		if plain=='quit':
			break

def client(port,G,mygroup):

	#connect to server address
	s = socket.socket()         
	host = socket.gethostname()               
	r=s.connect((host, port))
	print "connected"
	
	s.send(str(mygroup))
	dummy = s.recv(1024)

	#send auth msg to server
	secret='sworfish'
	msg='hello'
	tag=auth(secret,msg)
	authmsg=msg+"|"+tag
	s.send(authmsg)
	
	#receive reply from server
	buf = s.recv(1024)
	
	if buf=='Request accepted':
		#exchange public keys
		print buf
		server_pubkey=RSA.importKey(s.recv(10000))
		
		s.send(G.key[mygroup].publickey().exportKey(format='PEM'))
		
		#start communication
		t1=threading.Thread(name='sender',target=sender,args=(s,server_pubkey,mygroup))
		t1.start()
		t1.join()

	else:
		print buf


def server(c,addr,G,mygroup):
	
	group = int(c.recv(1024))
	c.send("dummy")
	
	header=str(addr)+" : "
	print "connection  received from ",addr
	
	#after accepting a connection receive an auth msg
	buf = c.recv(1024)   
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
		#exchange public keys
		c.send(G.key[group].publickey().exportKey(format='PEM'))
		keya=c.recv(10000)
		keya=RSA.importKey(keya)

		#start communication
		#t1=threading.Thread(name='sender',target=sender,args=(c,keya,))
		#t1.start()
		t2=threading.Thread(name='receiver',target=receiver,args=(c,G.key[group],))
		t2.start()
		#t1.join()
		t2.join()
	
	#else reject connection
	else:
		c.send("Request not authenticated")
		
	#close connection
	c.close()
		
def initser(G,port):
	s = socket.socket()         
	host = socket.gethostname()  
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	s.bind((host, port))   
	s.listen(5)    
	while True:
		#wait for connections
		c, addr = s.accept() 
		threading.Thread(name='server',target=server,args=(c,addr,G,mygroup,)).start()
		

if __name__=="__main__":
	
	#get port number from cmd

	port=int(sys.argv[1])
	mygroup=int(sys.argv[2])
	G=User()
	threading.Thread(name='initser',target=initser,args=(G,port,)).start()
	while True:
		print "start new connection enter 'serverport|group' :"
		p=raw_input()
		serv_port,mg=p.split('|')
		serv_port=int(serv_port)
		mg=int(mg)
		t1=threading.Thread(name='client',target=client,args=(serv_port,G,mg,))
		t1.start()
		t1.join()
	
	

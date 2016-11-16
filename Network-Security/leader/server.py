import socket
import threading
from Crypto.Hash import HMAC
from Crypto.PublicKey import RSA
import sys
import signal,os,select

#key for group A
class G(object):
	"""Cliet Group attributes for Server
	"""

	def __init__(self):
		self.key=RSA.generate(1024);
		#private key for the group(Text)
		self.pri_key=self.key.exportKey(format='PEM')
		#public key for the group(Text)
		self.pub_key=self.key.publickey().exportKey(format='PEM')
		#connection from clients
		self.mem=[]
		#pid of clients of group
		self.pid=[]
		#port of group leader
		self.groupport=0

	def gen_chunks(self,n):
		"""Partition the private key into n parts(no.of clients)
		"""
		
		#print pri_key
		length=len(self.pri_key)
		#print length
		chunk_size=length/n
		#print chunk_size
		parts=[]
		for i in range(0,length,chunk_size):
			#print i
			parts.append(self.pri_key[i:min(i+chunk_size,length)])

		#print parts
		return parts

	def pri_key_re_dist(self):
		"""Partition the private key and distribute among  connected clients of a group
		"""
		
		n=len(self.mem)
		print n
		print self.pri_key
		parts=self.gen_chunks(n)
		for i in range(0,len(self.mem)):
			os.kill(self.pid[i],signal.SIGUSR2);
			print "sent",parts[i]
			self.mem[i].send(parts[i])


		
def pub_key_dist(c,A,B):
	"""For each group send their public key first 
	and public key of other group next

	"""
	name=c.recv(4096);
	print name

	if name=='A':
		header=A.pub_key+'|'+B.pub_key+'|'+str(A.groupport)+'|'+str(B.groupport)
		c.send(header)
	elif name=='B':
		header=B.pub_key+'|'+A.pub_key+'|'+str(B.groupport)+'|'+str(A.groupport)
		c.send(header)
	print "sent values"

if __name__=="__main__":
	
	#get port number from cmd
	#gen_chunks(5)
	port=int(sys.argv[1])
	print os.getpid()

	#initialize two groups A and B
	A=G()
	B=G()
	#initialize their respective group ports
	A.groupport=int(sys.argv[2])
	B.groupport=int(sys.argv[3])

	#open a socket for serving incoming connections
	s = socket.socket()         
	host = socket.gethostname() 
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  
	s.bind((host, port))        
	s.listen(5)        
	s.setblocking(1)

	#accept group leader A 
	c_A,addr_A=s.accept()     
	print "connected with A"
	#give the public keys to group leader and port to open for group members
	t1=threading.Thread(name='sender',target=pub_key_dist,args=(c_A,A,B,))
	t1.start()

	#accept group leader B 
	c_B,addr_B=s.accept()    
	print "connected with B"  
	#give the public keys to group leader and port to open for group members
	t2=threading.Thread(name='sender',target=pub_key_dist,args=(c_B,A,B,))
	t2.start()
	#Let the key distribution finsih
	t1.join()
	t2.join()


	print "init complete"

	while True:
		#wait for connections
		print "waiting for memebers"
		c, addr = s.accept()
		print "connected with a member"
		buf=c.recv(4096) 
		pid,group = buf.split('|')
		#c.send(str(43567))
		print "received request"

		#add to given group and redistribute keys
		if group=='A':

			#server sends the port to which client has to connect depending on his group
			c.send(str(A.groupport))
			c.recv(4096) #an acknowledgement
			print "done"
			A.mem.append(c)
			A.pid.append(int(pid))
			A.pri_key_re_dist()
			
		elif group=='B':
			#server sends the port to which client has to connect depending on his group
			c.send(str(B.groupport))
			c.recv(4096)
			print "doneB"
			B.mem.append(c)
			B.pid.append(int(pid))
			B.pri_key_re_dist()
			


	
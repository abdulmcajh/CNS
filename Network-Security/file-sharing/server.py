import socket
import threading
from Crypto.Hash import HMAC
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
import sys
import os

key=RSA.generate(1024);

def auth(secret,msg):
	"""Calculate Auth tag( HMAC(k,M) ) for msg(M) using secret(K)

	"""
	h = HMAC.new(secret)
	h.update(msg)
	tag=h.hexdigest()
	return tag
	

                   
def server(c,addr):
	"""Servers a client connection
	Authorises a client and transfers the requested file encrypted using AES

	TODO:Reverse auth,key management
	"""
	print_header=str(addr)+" : "
	print "connection  received from ",addr
	
	#exchange public keys
	c.send(key.publickey().exportKey(format='PEM'))
	keya=RSA.importKey(c.recv(10000))

	#after accepting a connection receive an auth msg
	secret= key.decrypt(c.recv(1024))
	buf = key.decrypt(c.recv(1024))
	print print_header,"request received"
	#separate received message into msg and its auth tag
	msg,rtag= buf.split('|')
	print print_header,msg,rtag
	
	#find auth tag of received msg
	tag=auth(secret,msg)
	print print_header,tag

	#if tags equal accept connection
	if rtag==tag :

		c.send("Request accepted")
		
		#get filename requested by client and find its attributes
		req_filename=key.decrypt(c.recv(1024))
		filesize = os.path.getsize(req_filename)
		chunk_size=1024

		#choose key,mode and IV for AES
		aes_key = '0123456789abcdef'
		aes_IV = 16 * '\x00'         
		mode = AES.MODE_CBC
		encryptor = AES.new(aes_key, mode, IV=aes_IV)

		#send aes key and IV encrypted using PKC
		msg_header = aes_key+'|'+aes_IV+'|'+str(mode)+'|'+str(filesize)+'|'+str(chunk_size)
		c.send(keya.encrypt(msg_header,125)[0])

		sent_size=0
		#open the requested file and send parts of file encrypted using AES
		with open(req_filename, 'rb') as f:
			while sent_size<filesize:
				plain=f.read(chunk_size)
				c.send(encryptor.encrypt(plain))
				sent_size+=len(plain)

		
	
	#else reject connection
	else:
		c.send("Request not authenticated")
		
	#close connection
	c.close()
		
		
		

if __name__=="__main__":
	
	#get port number from cmd

	port=int(sys.argv[1])

	s = socket.socket()         # Create a socket object
	host = socket.gethostname() # Get local machine name
               
	s.bind((host, port))        # Bind to the port
	s.listen(5)                 
	while True:
		#wait for connections
		c, addr = s.accept() 
		#spawn a new thread for each received client connection
		threading.Thread(name='server',target=server,args=(c,addr)).start()

	
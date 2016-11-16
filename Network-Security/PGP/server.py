import socket
import threading
import zlib as zl
import base64 as b64
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
import sys
import os

key=RSA.generate(1024);


def PGP_pack(m,recv_pubkey):
	#find H(m)
	h = SHA256.new()
	h.update(m)
	tag=h.hexdigest()
	print tag
	#encrypt H(m)
	tag = key.encrypt(tag,125)[0]
	
	DS = m+ "|||"+tag
	print len(DS)
	DS = DS + (16-len(DS)%16)*"="
	print len(DS)
	print "DS",DS
	#z = zl.compress(DS, zl.Z_BEST_COMPRESSION)

	Ks = '0123456789abcdef'    
	mode = AES.MODE_CBC
	IV = 16 * '\x00'
	encryptor = AES.new(Ks,mode,IV=IV)
	cipher=encryptor.encrypt(DS)

	#encrypt Ks using Public key of receiver
	cipherkey_enc = recv_pubkey.encrypt(Ks,125)[0]

	total = cipher +"|-"+cipherkey_enc
	#print total
	#compress the total_msg
	#compressed_msg = zl.compress(total, zl.Z_BEST_COMPRESSION)
	#encode it using base64
	encoded_msg = b64.b64encode(total)
	#print encoded_msg
	return encoded_msg

def PGP_unpack(m):
	print m
	#decode from base64 to ascii
	decoded_msg = b64.b64decode(m)
	print decoded_msg
	#decompressed_msg= zl.decompress(decoded_msg)
	cipher,cipherkey_enc=decoded_msg.split('|-')
	#decrypt the pub_key encrypted Ks using private key
	Ks=key.decrypt(cipherkey_enc)
	print Ks
	print len(cipher)
	mode = AES.MODE_CBC
	IV = 16 * '\x00'
	decryptor = AES.new(Ks, mode,IV=IV)
	DS=decryptor.decrypt(cipher)

	msg,tag=DS.split('|-')
	rtag=key.decrypt(tag)
	
	h = SHA256.new()
	h.update(msg)
	tag=h.hexdigest()

	if tag==rtag:
		print "Accepted"
		return msg

	else:
		print "Rejected"

def sender(s,server_pubkey):
	while True:
		m=raw_input()
		s.send(PGP_pack(m,server_pubkey))
		

def receiver(s):
	while True:
		text=s.recv(4096)
		print "----Received",PGP_unpack(text)

def server(c,addr):
	"""Servers a client connection
	Authorises a client and transfers the requested file encrypted using AES

	TODO:Reverse auth,key management
	"""
	print_header=str(addr)+" : "
	print "connection  received from ",addr
	
	#exchange public keys
	c.send(key.publickey().exportKey(format='PEM'))
	cli_pubkey=RSA.importKey(c.recv(10000))

	p1=threading.Thread(name='sender',target=sender,args=(c,cli_pubkey,))
	p1.start()
	p2=threading.Thread(name='receiver',target=receiver,args=(c,))
	p2.start()
	
	p1.join()
	p2.join()
	
		
		
		

if __name__=="__main__":
	
	#get port number from cmd

	port=int(sys.argv[1])

	s = socket.socket()         
	host = socket.gethostname() 
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	s.bind((host, port))      
	s.listen(5)                 
	while True:
		#wait for connections
		c, addr = s.accept() 
		#spawn a new thread for each received client connection
		threading.Thread(name='server',target=server,args=(c,addr)).start()

	

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
	#encrypt H(m)
	tag = key.encrypt(tag,125)[0]

	DS = m+ "|-"+tag

	print len(DS)
	DS = DS + (16-len(DS)%16)*"="
	print len(DS)
	#z = zl.compress(DS, zl.Z_BEST_COMPRESSION)

	Ks = '0123456789abcdef'    
	mode = AES.MODE_CBC
	IV = 16 * '\x00'
	encryptor = AES.new(Ks,mode,IV=IV)
	cipher=encryptor.encrypt(DS)

	#encrypt Ks using Public key of receiver
	cipherkey_enc = recv_pubkey.encrypt(Ks,125)[0]

	total = cipher +"|-"+cipherkey_enc
	print total
	#compress the total_msg
	#compressed_msg = zl.compress(total, zl.Z_BEST_COMPRESSION)
	#encode it using base64
	encoded_msg = b64.b64encode(total)
	print encoded_msg
	return encoded_msg

def PGP_unpack(m):
	#print m
	#decode from base64 to ascii
	decoded_msg = b64.b64decode(m)
	#print decoded_msg
	#decompressed_msg= zl.decompress(decoded_msg)
	cipher,cipherkey_enc=decoded_msg.split('|-')
	#decrypt the pub_key encrypted Ks using private key
	Ks=key.decrypt(cipherkey_enc)
	print Ks
	mode = AES.MODE_CBC
	IV = 16 * '\x00'
	decryptor = AES.new(Ks, mode,IV=IV)
	DS=decryptor.decrypt(cipher)
	#print "DS",DS
	msg,tag=DS.split('|||')
	tag=tag.strip('=')
	#print 'TAG\n',tag 
	rtag=key.decrypt(tag)
	print "rtag ",rtag
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

def client(port):
	"""Connect to server. Authorize to server. Request a file. Download that file

	port : server port number
	"""
	#connect to server address
	s = socket.socket()         
	host = socket.gethostname()               
	r=s.connect((host, port))
	print "connected"

	#exchange public keys
	server_pubkey=RSA.importKey(s.recv(10000))
	s.send(key.publickey().exportKey(format='PEM'))

	p1=threading.Thread(name='sender',target=sender,args=(s,server_pubkey,))
	p1.start()
	p2=threading.Thread(name='receiver',target=receiver,args=(s,))
	p2.start()
	
	p1.join()
	p2.join()
	
                       

if __name__=="__main__":

	port=int(sys.argv[1])
	t1 = threading.Thread(name='client', target=client, args=(port,))
	t1.start()
	t1.join()
	

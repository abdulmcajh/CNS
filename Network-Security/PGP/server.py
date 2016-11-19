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
	tag = recv_pubkey.encrypt(tag,125)[0]

	#DS= m + tag
	DS = m+ "|-"+tag

	#pad to make length a multiple of 16 for AES encryption
	DS = DS + (16-len(DS)%16)*"="

	#z = zl.compress(DS, zl.Z_BEST_COMPRESSION)
	
	#Encrypt cipher using AES with key Ks
	Ks = '0123456789abcdef'    
	mode = AES.MODE_ECB
	encryptor = AES.new(Ks, mode)
	cipher=str(encryptor.encrypt(DS))

	#encrypt Ks using Public key of receiver
	cipherkey_enc = recv_pubkey.encrypt(Ks,125)[0]

	total_msg = cipher +"|-"+cipherkey_enc
	#compress the total_msg
	compressed_msg = zl.compress(total_msg, zl.Z_BEST_COMPRESSION)
	#encode it using base64
	encoded_msg = b64.b64encode(compressed_msg)

	return encoded_msg

def PGP_unpack(m):
	#decode from base64 to ascii
	decoded_msg = b64.b64decode(m)

	#decompress the message
	decompressed_msg= zl.decompress(decoded_msg)
	#separeate cipher and cipherkey_enc
	cipher,cipherkey_enc=decompressed_msg.split('|-')

	#decrypt the public key encrypted "Ks" using private key
	Ks=key.decrypt(cipherkey_enc)

	#Decrypt cipher using AES with key Ks
	mode = AES.MODE_ECB
	decryptor = AES.new(Ks, mode)
	DS=decryptor.decrypt(cipher)
	#strip the padded characters
	DS=DS.strip('=')
	
	#separeate msg and tag
	msg,tag=DS.split('|-')

	#decrypt the public key encrypted "tag" using private key
	rtag=key.decrypt(tag)
	
	#hash the received message
	h = SHA256.new()
	h.update(msg)
	tag=h.hexdigest()

	#compare tags to authenticate
	if tag==rtag:
		print "Accepted PGP message"
		return msg
	else:
		print "Rejected message"

def sender(s,server_pubkey):
	while True:
		m=raw_input()
		print "sending the message to other using PGP packing.."
		cipher=PGP_pack(m,server_pubkey)
		print "Output from PGP\n",cipher
		s.send(cipher)
		print "**Message sent"
		if m=="quit":
			break
		

def receiver(s):
	while True:
		text=s.recv(4096)
		print "--Received PGP encoded_msg \n",text
		plain =PGP_unpack(text)
		print "--After PGP unpacking \n",plain
		if plain=="quit":
			break


def server(c,addr):
	
	print_header=str(addr)+" : "
	print "connection  received from ",addr
	print "Type in Console to send message to other.Press 'quit' to stop communication"
	
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

	
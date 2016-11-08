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

	#send secret used for auth using PKC
	secret='sworfish'
	s.send(server_pubkey.encrypt(secret,125)[0])

	#send auth msg to server
	msg='hello'
	tag=auth(secret,msg)
	authmsg=msg+"|"+tag
	s.send(server_pubkey.encrypt(authmsg,125)[0])
	
	#receive reply from server
	reply= s.recv(1024)
	
	if reply=='Request accepted':
		
		#enter the filename you want to download
		req_filename=raw_input("Enter the filename :")
		s.send(server_pubkey.encrypt(req_filename,125)[0])

		#get the header sent by server
		ciphered_header=s.recv(10000)
		header= key.decrypt(ciphered_header)
		#get all attributes for file transfer and covert into respective types
		aes_key,aes_IV,aes_mode,filesize,chunk_size = header.split('|')
		aes_mode = int(aes_mode)
		filesize=int(filesize)
		chunk_size=int(chunk_size)

		#create a AES object with parameters sent by server
		decryptor = AES.new(aes_key, aes_mode, IV=aes_IV)
		#filename into which content is stored on client side
		store_filename="kk"+req_filename
		recv_size=0
		#open outfile and write the contents received from server until we receive filesize no.of bytes
		with open(store_filename, 'wb') as f:
			while recv_size<filesize:
				plain = decryptor.decrypt(s.recv(chunk_size))
				f.write(plain)
				recv_size+=chunk_size

	else:
		print reply


	#close connection
	s.close()
	
                       

if __name__=="__main__":

	port=int(sys.argv[1])
	t1 = threading.Thread(name='client', target=client, args=(port,))
	t1.start()
	t1.join()
	
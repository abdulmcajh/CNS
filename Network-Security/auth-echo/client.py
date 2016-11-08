import socket
import threading
from Crypto.Hash import HMAC
from Crypto.PublicKey import RSA
import sys

key=RSA.generate(1024);

def auth(secret,msg):
	h = HMAC.new(secret)
	h.update(msg)
	tag=h.hexdigest()
	return tag
	
	 

def client(port):

	#connect to server address
	s = socket.socket()         
	host = socket.gethostname()               
	r=s.connect((host, port))
	print "connected"

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
		server_pubkey=RSA.importKey(s.recv(10000))
		
		s.send(key.publickey().exportKey(format='PEM'))

		#start communication , To end communication send 'quit'
		while True:
			p=raw_input()
			cipher=server_pubkey.encrypt(p,123)
			s.send(cipher[0])
			if p=='quit':
				break;
			print key.decrypt(s.recv(10000))

	else:
		print buf


	#close connection
	s.close()
	
                       

if __name__=="__main__":

	port=int(sys.argv[1])
	t1 = threading.Thread(name='client', target=client, args=(port,))
	t1.start()
	t1.join()
	
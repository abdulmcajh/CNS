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
	

                   
def server(c,addr):
	
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
		c.send(key.publickey().exportKey(format='PEM'))
		keya=c.recv(10000)
		keya=RSA.importKey(keya)

		#start communication
		while 1:
			
			cipher=c.recv(10000)
			
			plain= key.decrypt(cipher)

			if plain=='quit':
				print header,"connection terminated"
				break
			print header,plain
			c.send(keya.encrypt(plain,125)[0])
	
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
		threading.Thread(name='server',target=server,args=(c,addr)).start()

	
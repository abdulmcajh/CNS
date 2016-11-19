import socket
import threading
from Crypto import Random
from Crypto.Cipher import AES
import sys


#fixed long term keys
K_tgs = b'Sixteen byte key'
K_v = b'Mount Everestkey'
K_c = b'Yellow stone key'

def encrypt(plaintext,key):
	length = len(plaintext)
	
	#padding to make lenght multiple of 16 for AES
	if length%16!=0 :
		plaintext =plaintext + "="*(16 - length%16)
	
	#find the cipher of the plaintext
	encryptor = AES.new(key, AES.MODE_ECB)
	cipher = encryptor.encrypt(plaintext)
	return str(cipher)


def decrypt(ciphertext,key):
	decryptor = AES.new(key, AES.MODE_ECB)
	plain = decryptor.decrypt(ciphertext)
	#strip padded bits
	return plain.strip('=')

def server(c,addr):

	print_header=str(addr)+" : "
	print "connection  received from ",addr

	#Receive request from C
	buf = c.recv(4096)
	print "Received request:\n",buf
	IDc,IDtgs,TS1 = buf.split('|&|')

	lifetime =str(255)
	ADc = str(addr[0])
	TS2=2

	#generate K_c_tgs
	K_c_tgs= str(Random.new().read(AES.block_size))

	raw_ticket_tgs=K_c_tgs+"|&|"+IDc+"|&|"+ADc+"|&|"+IDtgs+"|&|"+str(TS2)+"|&|"+lifetime
	#encrypt raw_ticket using K_tgs
	ticket_tgs = encrypt(raw_ticket_tgs,K_tgs)

	#send a message encrypted using K_c to C which contains the ticket_tgs
	msg = K_c_tgs+"|&|"+IDtgs+"|&|"+str(TS2)+"|&|"+lifetime+"|&|"+ticket_tgs

	enc_msg= encrypt(msg,K_c)

	c.send(enc_msg)

	print "Sent Reply to C :\n",enc_msg
	print "Complete serving C"
	
	

if __name__=="__main__":
	
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
         

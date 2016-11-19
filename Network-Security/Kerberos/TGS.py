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

	#Note the connection IP address
	RADc = str(addr[0])

	#receive request from C
	buf = c.recv(4096)
	print "Received request:\n",buf

	IDv,ticket_tgs,authenticator_c = buf.split('|&|')

	#decrypt the ticket_tgs given by C
	raw_ticket_tgs = decrypt(ticket_tgs,K_tgs)

	#get K_c_tgs
	K_c_tgs,IDc,ADc,IDtgs,TS2,lifetime = raw_ticket_tgs.split('|&|')

	#check if the IP which requested ticket to TGS from AS is same the current
	if ADc==RADc:
		print "TGS Accepted the request by C to connect to V"
	
		TS4=4

		#generate K_c_v
		K_c_v= str(Random.new().read(AES.block_size))
		raw_ticket_v=K_c_v+"|&|"+IDc+"|&|"+ADc+"|&|"+IDv+"|&|"+str(TS4)+"|&|"+lifetime
		#encrypt raw_ticket using K_v
		ticket_v = encrypt(raw_ticket_v,K_v)

		#send a message encrypted using K_c_tgs to C which contains the ticket_v
		msg = K_c_v+"|&|"+IDv+"|&|"+str(TS4)+"|&|"+lifetime+"|&|"+ticket_v

		print "Message for C (plain)\n",msg
		enc_msg= encrypt(msg,K_c_tgs)

		c.send(enc_msg)

		print "Sent Reply to C\n",enc_msg
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

	


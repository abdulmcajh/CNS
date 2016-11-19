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
	ticket_v,authenticator_c = buf.split('|&|')

	#decrypt the ticket using K_v
	K_c_v, IDc, ADc, IDv, TS4 , lifetime = decrypt(ticket_v,K_v).split('|&|')

	#decrypt the authenticator using K_c_v
	auth = decrypt(authenticator_c,K_c_v)
	IDc,ADc,TS6 = auth.split('|&|')

	#check if the IP addresses match
	if ADc==RADc:
		print "V Accepted the request by C "
		msg = str(int(TS6)+1)

		#send a message to C so that it can authenticate V
		enc_msg= encrypt(msg,K_c_v)
		c.send(enc_msg)
		print "Sent reply to C:\n",enc_msg


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
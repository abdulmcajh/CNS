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

if __name__=="__main__":
	
	#port of AS
	sport=int(sys.argv[1])
	#port of TGS
	tport=int(sys.argv[2])
	#port of V
	vport=int(sys.argv[3])

	#let IDc  = port number
	IDc = str(1234)
	IDtgs = str(tport)
	IDv = str(vport)
	lifetime = str(255)
	TS1 = 1;

	#Connect to AS
	s = socket.socket()         
	host = socket.gethostname() 
	s.connect((host, sport))
	#get IP address of own
	ADc = s.getsockname()[0]; 
	#print ADc
	print "connected to AS"
	#Request AS for ticket to TGS
	request = IDc+"|&|"+IDtgs+"|&|"+str(TS1)
	print "sending Request to AS : \n",request
	s.send(request)
	#receive reply from AS
	reply=s.recv(4096)
	#Decrpyt using K_c and get ticket_tgs and K_c_tgs
	reply = decrypt(reply,K_c)
	print "receivedr reply from AS : \n",reply
	K_c_tgs, IDtgs, TS2, lifetime, ticket_tgs = reply.split('|&|')

	
	TS3=3
	#prepare authenticator_c using K_c_tgs got from AS
	auth_msg = IDc+"|&|"+ADc+"|&|"+str(TS3)
	
	authenticator_c= encrypt(auth_msg,K_c_tgs)

	#Connect to TGS
	t = socket.socket()         
	host = socket.gethostname() 
	t.connect((host, tport))
	print "connected to TGS"
	#Request TGS for ticket to V
	request = IDv+"|&|"+ticket_tgs+"|&|"+authenticator_c
	print "sending Request to TGS : \n",request
	t.send(request)
	#receive reply from TGS
	reply=t.recv(4096)
	#Decrpyt using K_c_tgs and get ticket_v and K_c_v
	reply = decrypt(reply,K_c_tgs)
	print "receivedr reply from TGS : \n",reply
	K_c_v, IDv, TS4, lifetime, ticket_v = reply.split('|&|')
	
	#prepare authenticator_c using K_c_v got from TGS
	TS6=6
	auth_msg = IDc+"|&|"+ADc+"|&|"+str(TS6)

	authenticator_c= encrypt(auth_msg,K_c_v)

	#Connect to V
	v = socket.socket()         
	host = socket.gethostname() 
	v.connect((host, vport))
	print "connected to V"
	#Give ticket to V to authenticate 
	request = ticket_v+"|&|"+authenticator_c
	print "sending Request to V : \n",request
	v.send(request)
	#receive reply from V
	reply=v.recv(4096)
	reply = decrypt(reply,K_c_v)
	if int(reply)==TS6+1 :
		print "V authenticated"
		print "Auth Completed"
	


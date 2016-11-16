Execution:

python server.py 50004

python client.py 50004

Explaination:

server accepts connections on 50004

after a client connection .

the client is authenticated by server and public keys are exchanged

the server sends the parameters of encryption  of file using AES to client . encrypted by client public key

the server sends encrypted chunks of file ,"requested file should be in the directory"

the client decrypts the chunks and writes into another file in the same directory with name kk'filename'


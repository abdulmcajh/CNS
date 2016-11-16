**Execution:**

* python server.py 50004

* python client.py 50004

**Explaination:**

1. server accepts connections on 50004

2. after a client connection .

3. the client is authenticated by server and public keys are exchanged

4. the server sends the parameters of encryption  of file using AES to client . encrypted by client public key

5. the server sends encrypted chunks of file ,"requested file should be in the directory"

6. the client decrypts the chunks and writes into another file in the same directory with name kk'filename'


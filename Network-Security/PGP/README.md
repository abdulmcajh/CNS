##PGP

### parameters
serv_port = 32004

**Execution**
* python server.py server_port
* python client.py server_port

**Explanation**

* server opens port "server_port" and accepts connections
* client connects to the server
* public keys are exchanged
* Each one can communicate with other through console input
* Each message goes through PGP protocol steps and output is sent to other
* the other applys PGP protocol and checks if message is as sent from sender
* if accepted writes the received message to the console


##Kerberos

* AS : Authentication Server
* TGS : Ticket Granting Server
* C : Client
* V : Server

### parameters
* sport = port of AS = 32001
* tport = port of TGS = 32002
* vport = port of v = 32003

**Execution**
* python AS.py sport
* python TGS.py tport
* python V.py vport
* python C.py sport tport vport

**Explanation**
* Initially we assume the long termm keys K_c,K_tgs,K_v are already distributed
* short term keys K_c_v and K_c_tgs are random generated and distributed
* C requests AS a ticket to TGS
* After receiving ticket to TGS, C requests TGS to grant a ticket to V
* After receiving tivket to V , C gives the tickets to V.
* V verify the ticket.If accepted (authentication of C completed)  V sends a message to C
* C receives and verifies.If accepted (authentication of V completed)
* Therefore Authentication is Complete using Kerberos

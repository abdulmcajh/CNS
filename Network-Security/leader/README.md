**Parameters**
server port :23455
 
leader A port :23457
 
leader B port :23458

**Execution:**

**server:**

* python server.py 23455 23457 23458

**leader A:**

* python leader.py 23455 A

**leader B:*

* python leader.py 23455 B

**client of type A:**

* python normal.py 23455 A

**client of type B:**

* python normal.py 23455 B


**Running explanation:**


*Intially happend after starting above commads:*

- server initially generates 2 keys for two groups

- gives the leaders public keys of their respective group

- group leaders exchange their public keys with each other

- when a normal client of type come to server it reditributes private key of that group among its group members



*After all above programs are waiting:*

1. In the console of a normal client type a message 

2. the message goes to the group leader

3. he encrypts it with other group leader  public key and sends to  other group leader

4. the other group leader on receiving the message collects private key from its members and decrypts the message.it is printed in his console

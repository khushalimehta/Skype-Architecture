# Skype Architecture
# Authentication Server (name of the file: server1.c)
•	Compile the file with command, gcc server1.c –o s
•	Run the file by ./s 

# Supernode Server (name of the file: supernode_copy.c)
•	Compile the file with command gcc supernode_copy.c –o sn –lpthread
•	Run the file by mentioning any port number, ./sn “portno 1”
•	Enter another port number for supernode to supernode connection. “portno 2”
•	 * ENTER THE OTHER ACTIVE SUPERNODE IP AND “portno 2”.

# Client (name of the file: client2.c)
•	Compile the file with command, gcc client2.c –o c
•	Run the file by ./c “IP of authentication server” “6666(by default port number for authentication server)”
•	Select ‘a’ for login and ‘b’ for registration.
•	After registration and login, register to supernode server by entering IP address and “portno 1” of supernode.
•	Select ‘a1’ for known user and ‘b1’ for new user. For first time, we need to select ‘b1’ as there will be no users stored in an array. (Arrays of supernode and client aren’t persistent, so once they exit the program then their entries will be deleted from the array) [* CHECK SUPERNODE TERMINAL AFTER THIS STEP]
•	If ‘b1’ then enter the name of the username you want to chat with (Username is assumed to be known). That username should be registered with one of the supernode.
•	Enter remote port (which will be local for another online user you want to chat with) and enter local port (assume any port number). Simultaneously other user will also do the same. Once the port numbers matches, connection is established which is peer to peer. Start chatting. 
•	Press ‘EXIT’ to end the chat. It will ask for known user or new user step once again.

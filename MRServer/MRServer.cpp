
const char * usage =
"                                                               \n"
"MRServer:                                                      \n"
"                                                               \n"
"Simple server program used to communicate MIDI information.    \n"
"                                                               \n"
"To use it, type:                               		\n"
"                                                               \n"
"   MRServer <port>                                             \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include "MRServer.h"

using namespace std;

int QueueLength = 5;

//test

//Room vector
//vector<Room> rooms;

int
MRServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);
  
	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1; 
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
			     (char *) &optval, sizeof( int ) );
	
	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			  (struct sockaddr *)&serverIPAddress,
			  sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}
	
	// Put socket in listening mode and set the 
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

void
MRServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);

	//initialize();
	
	while ( 1 ) {
		
		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
					  (struct sockaddr *)&clientIPAddress,
					  (socklen_t*)&alen);
		
		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}
		
		// Process request.
		processRequest( slaveSocket );		
	}
}

int
main( int argc, char ** argv )
{
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}
	
	// Get the port from the arguments
	int port = atoi( argv[1] );

	MRServer MRServer;

	// It will never return
	MRServer.runServer(port);
	
}

void
MRServer::processRequest( int fd )
{
	// Buffer used to store the comand received from the client
	//const int MaxCommandLine = 1024;
	//char commandLine[ MaxCommandLine + 1 ];
	//int commandLineLength = 0;
	//int n;
	
	// Currently character read
	//unsigned char prevChar = 0;
	unsigned char newByte = 0;
	unsigned char *buffer = new unsigned char[4]; 
	int index = 0;
	
	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( read( fd, &newByte, 1) > 0 ) {

		if (index == 4) {
			index = 0;
		}

		buffer[index] = newByte;
		index++;

		if (index == 4) {
			write(fd, buffer, sizeof(unsigned char) * 4);
		}
		
	}
}	
/*	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;

	printf("RECEIVED: %s\n", commandLine);

	printf("The commandLine has the following format:\n");
	printf("COMMAND <user> <password> <arguments>. See below.\n");
	printf("You need to separate the commandLine into those components\n");
	printf("For now, command, user, and password are hardwired.\n");

	//const char * command = "ADD-USER";
	//const char * user = "peter";
	//const char * password = "spider";
	//const char * args = "";
	
	char commandLineElements[4][50];
	int countElem = 0;
	int currentChar = 0;
	for (int i = 0; i < commandLineLength; i++) {
		if (countElem <= 2) {
			if (commandLine[i] == ' ' || commandLine[i] == '\n') {
				commandLineElements[countElem][currentChar] = '\0';
				countElem++;
				currentChar = 0;
			} else {
				commandLineElements[countElem][currentChar] = commandLine[i];
				currentChar++;
			}
		} else if (countElem == 3) {
			if (commandLine[i] != '\n') {
				commandLineElements[countElem][currentChar] = commandLine[i];
				currentChar++;
			} else {
				commandLineElements[countElem][currentChar] = '\0';
			}
		} 
	}
	commandLineElements[countElem][currentChar] = '\0';

	const char * command = commandLineElements[0];
	const char * user = commandLineElements[1];
	const char * password = commandLineElements[2];
	const char * args = commandLineElements[3];

	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else if (!strcmp(command, "CREATE-ROOM")) {
		createRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		createRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LIST-ROOMS")) {
		listRooms(fd, user, password, args);
	}
	else {
		const char * msg = "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	//Send OK answer
	//const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);	
}

void
MRServer::initialize()
{
	// Open password file

	// Initialize users in room

	// Initalize message list

	vector<Room> rooms;

}

bool
MRServer::checkPassword(int fd, const char * user, const char * password) {
	// Here check the password
	FILE * pwdList = fopen("password.txt","r");
	int countLines = 1;
	int userExists = 0;
	char currentLine[100];
	fgets(currentLine,100,pwdList);
	for (int i = 0; i < 100; i++) {
		if (currentLine[i] == '\n') {
			currentLine[i] = '\0';
		}
	}
	while(!feof(pwdList)) {
		if(countLines % 3 == 1 && strcmp(currentLine,user) == 0) {
			fgets(currentLine, 100, pwdList);
			for (int j = 0; j < 100; j++) {
				if (currentLine[j] == '\n') {
					currentLine[j] = '\0';
				}
			}
			if (!strcmp(currentLine,password)) {
				return true;
			} else {
				write(fd,"ERROR (Wrong password)\r\n", strlen("ERROR (Wrong password)\r\n"));
				return false;
			}
		}
		fgets(currentLine, 100, pwdList);
		for (int k = 0; k < 100; k++) {
			if (currentLine[k] == '\n') {
				currentLine[k] = '\0';
			}
		}
		countLines++;
	}

	write(fd,"ERROR (Wrong password)\r\n", strlen("ERROR (Wrong password)\r\n"));
	return false;
}

void
MRServer::addUser(int fd, const char * user, const char * password, const char * args)
{
		FILE * pwdList = fopen("password.txt","a+");
		int countLines = 1;
		int nameTaken = 0;
		char currentLine[100];
		fgets(currentLine,100,pwdList);
		for (int i = 0; i < 100; i++) {
			if (currentLine[i] == '\n') {
				currentLine[i] = '\0';
			}
		}
		while(!feof(pwdList)) {
			if(countLines % 2 == 1 && strcmp(currentLine,user) == 0) {
				nameTaken = 1;
			}
			fgets(currentLine, 100, pwdList);
			for (int j = 0; j < 100; j++) {
				if (currentLine[j] == '\n') {
					currentLine[j] = '\0';
				}
			}
			countLines++;
		}

		if (nameTaken == 0) {
			fputs(user,pwdList);
			fputc('\n',pwdList);
			fputs(password,pwdList);
			fputc('\n',pwdList);
			fputc('\n',pwdList);
			fclose(pwdList);
			const char * msg =  "OK\r\n";
			write(fd, msg, strlen(msg));
		}		
}

void
MRServer::enterRoom(int fd, const char * user, const char * password, const char * args)
{
/*	//TODO: Delete between comments
	FILE * pwdList = fopen("password.txt","r");
	int countLines = 1;
	int userExists = 0;
	char currentLine[100];
	fgets(currentLine,100,pwdList);
	while(!feof(pwdList)) {
		if(countLines % 2 == 1 && !strcmp(currentLine,user)) {
			userExists = 1;
		}
		fgets(currentLine, 100, pwdList);
		countLines++;
	}
	//
	if(checkPassword(fd,user,password)){ //TODO: Replace with checkPassword
		int roomExists = 0;
		for (int i = 0; i < rooms.size(); i++) {
			if (!strcmp(rooms[i].name.c_str(), args)) {
				int alreadyIn = 0;
				roomExists = 1;
				for (int j = 0; j < rooms[i].users.size(); j++) {
					if (!strcmp(rooms[i].users[j].c_str(), user)) {
						alreadyIn = 1;
					} 
				}
				if (alreadyIn == 0) {
					rooms[i].users.push_back(user);
					sort(rooms[i].users.begin(),rooms[i].users.end());
					const char * msg =  "OK\r\n";
					write(fd, msg, strlen(msg));
					return;
				}
				const char * msg =  "OK\r\n";
				write(fd, msg, strlen(msg));
				return;	
			}
//			write(fd,"ERROR (No room)\r\n",strlen("ERROR (No room)\r\n"));
		}
		if (roomExists == 0) {
			write(fd,"ERROR (No room)\r\n",strlen("ERROR (No room)\r\n"));
		}
	}
}

void
MRServer::leaveRoom(int fd, const char * user, const char * password, const char * args)
{

	if(checkPassword(fd,user,password)){ //TODO: Replace with checkPassword
		for (int i = 0; i < rooms.size(); i++) {
			if (!strcmp(rooms[i].name.c_str(), args)) {
				int userInRoom = 0;
				for (int j = 0; j < rooms[i].users.size(); j++) {
					if (!strcmp(rooms[i].users[j].c_str(),user)) {
						userInRoom = 1;
						rooms[i].users.erase(rooms[i].users.begin()+j);
						sort(rooms[i].users.begin(),rooms[i].users.end());
						const char * msg =  "OK\r\n";
						write(fd, msg, strlen(msg));
						return;
					}
					//const char * msg = "ERROR (No user in room)\r\n";
					//write(fd,msg,strlen(msg));
				}
				if (userInRoom == 0) {
					write(fd,"ERROR (No user in room)\r\n",strlen("ERROR (No user in room)\r\n"));
				}
			}
		}
	}
}

void
MRServer::sendMessage(int fd, const char * user, const char * password, const char * args)
{

	if(checkPassword(fd,user,password)){
		string message = args;
		string splitChar = " ";
		string room = message.substr(0, message.find(splitChar));	
		message.erase(0, message.find(splitChar)+1);

		for (int i = 0; i < rooms.size(); i++) {
			if (!strcmp(rooms[i].name.c_str(), room.c_str())) {
				int userInRoom = 0;
				for (int j = 0; j < rooms[i].users.size(); j++) {
					if (!strcmp(rooms[i].users[j].c_str(),user)){
						userInRoom = 1;
					}
				}
				if (userInRoom == 0) {
					write(fd,"ERROR (user not in room)\r\n", strlen("ERROR (user not in room)\r\n"));
					return;
				}
				Message mess;
				mess.message = message;
				mess.user = user;
				rooms[i].messages.push_back(mess);
				
				const char * msg =  "OK\r\n";
				write(fd, msg, strlen(msg));
				return;
			}
		}	
	}

}

void
MRServer::getMessages(int fd, const char * user, const char * password, const char * args)
{

	if(checkPassword(fd,user,password)){
		string roomName = args;
		string splitChar = " ";
		string numStart = roomName.substr(0, roomName.find(splitChar));	
		roomName.erase(0, roomName.find(splitChar)+1);

		int numStartInt;
		sscanf(numStart.c_str(), "%d", &numStartInt);

		for (int i = 0; i < rooms.size(); i++) {
			if (!strcmp(rooms[i].name.c_str(), roomName.c_str())) {
				int userInRoom = 0;
				for (int j = 0; j < rooms[i].users.size(); j++) {
					if (!strcmp(rooms[i].users[j].c_str(),user)){
						userInRoom = 1;
					}
				}
				if (userInRoom == 0) {
					write(fd,"ERROR (User not in room)\r\n", strlen("ERROR (User not in room)\r\n"));
					return;
				}
				if (rooms[i].messages.size() > numStartInt) {
					for (int j = numStartInt + 1; j < rooms[i].messages.size(); j++) {
						string numChar;
						stringstream temp;
						temp << j;
						numChar = temp.str();
						write(fd, numChar.c_str(), strlen(numChar.c_str()));
						write(fd, " ", strlen(" "));
						write(fd, rooms[i].messages[j].user.c_str(), strlen(rooms[i].messages[j].user.c_str()));
						write(fd, " ", strlen(" "));
						write(fd, rooms[i].messages[j].message.c_str(), strlen(rooms[i].messages[j].message.c_str()));

						write(fd, "\r\n", strlen("\r\n"));
					}
				} else {
					write(fd, "NO-NEW-MESSAGES", strlen("NO-NEW-MESSAGES"));
				}
				write(fd,"\r\n", strlen("\r\n"));
				return;
			}
//			write(fd,"ERROR (User not in room)\r\n", strlen("ERROR (User not in room)\r\n"));
		}
	}

}

void
MRServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args)
{
	if(checkPassword(fd,user,password)) {
	for (int i = 0; i < rooms.size(); i++) {
		if (!strcmp(rooms[i].name.c_str(), args)) {
			for (int j = 0; j < rooms[i].users.size(); j++) {
				write(fd,rooms[i].users[j].c_str(),strlen(rooms[i].users[j].c_str()));
				write(fd,"\r\n",strlen("\r\n"));
			}
			write(fd,"\r\n",strlen("\r\n"));
		}
	}
	}
}

void
MRServer::getAllUsers(int fd, const char * user, const char * password,const  char * args)
{
	if(checkPassword(fd,user,password)){
	FILE * pwdList = fopen("password.txt","a+");
	int countLines = 1;
	char currentLine[100];
	vector<string> userList;
	while(!feof(pwdList)) {
		fgets(currentLine, 100, pwdList);
		if(countLines % 3 == 1) {
			userList.push_back(currentLine);
		}
		
		countLines++;
	}
	fclose(pwdList);

	//write(fd, userList[0], strlen(userList[0]));

	sort(userList.begin(), userList.end()-1);
	
	//write(fd, userList[0], strlen(userList[0]));
	for (int i = 0; i < userList.size()-1; i++) {
		userList[i].erase(userList[i].size()-1);
		write(fd,userList[i].c_str(),strlen(userList[i].c_str()));
		write(fd,"\r\n",strlen("\r\n"));
	}
	write(fd,"\r\n",strlen("\r\n"));
	}
}

void
MRServer::createRoom(int fd, const char * user, const char * password, const char * args)
{
	if(checkPassword(fd,user,password)) {
	int roomExists = 0;
	for (int i = 0; i < rooms.size(); i++) {
		if (!strcmp(rooms[i].name.c_str(), args)) {
			roomExists = 1;	
		}
	}
	if (!roomExists) {
		Room room;
		room.name = args;
		rooms.push_back(room);
		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
	}
}


void
MRServer::listRooms(int fd, const char * user, const char * password,const  char * args)
{
	if(checkPassword(fd,user,password)){
		for (int i = 0; i < rooms.size(); i++) {
			write(fd,rooms[i].name.c_str(),strlen(rooms[i].name.c_str()));
			write(fd,"\r\n",strlen("\r\n"));
		}
		write(fd,"\r\n",strlen("\r\n"));
	}
}
*/

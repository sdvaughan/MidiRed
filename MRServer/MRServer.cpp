
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
	int masterSocket2 = open_server_socket(port+1);

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
		
		struct sockaddr_in clientIPAddress2;
		int alen2 = sizeof( clientIPAddress2 );
		int slaveSocket2 = accept( masterSocket2,
					  (struct sockaddr *)&clientIPAddress2,
					  (socklen_t*)&alen2);
		
		if ( slaveSocket2 < 0 ) {
			perror( "accept" );
			exit( -1 );
		}
		

		// Process request.
		processRequest( slaveSocket, slaveSocket2 );		
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
MRServer::processRequest( int fd, int fd2 )
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
			write(fd2, buffer, sizeof(unsigned char) * 4);
		}
		
	}
}	

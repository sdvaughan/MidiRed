
#ifndef MR_SERVER
#define MR_SERVER


class MRServer {
	// Add any variables you need

private:
	int open_server_socket(int port);

public:
		

	//void initialize();
	void runServer(int port);
	void processRequest( int socket );
};

#endif

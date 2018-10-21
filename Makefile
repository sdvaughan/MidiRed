# MRServer
MRS: ./MRServer/MRServer.h ./MRServer/MRServer.cpp 
	g++ -g -o ./bin/MRServer ./MRServer/MRServer.cpp
	cp ./bin/MRServer ./MRServer/MRServer

clean:
	$(RM) ./MRServer/*.o ./MRServer/MRServer 

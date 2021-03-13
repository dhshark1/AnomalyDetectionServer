#include "Server.h"

Server::Server(int port)throw (const char*) {
    fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (fileDescriptor < 0) {
        throw "socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if(::bind(fileDescriptor, (struct sockaddr*)&server, sizeof(server)) < 0)
        throw "bind failure";
    if(listen(fileDescriptor, 1) < 0)
        throw "listen failure";

}

void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch, this]{
        while (!stopLoop) {
            socklen_t clientSize = sizeof(client);
            int aClient = accept(fileDescriptor, (struct sockaddr*)&client, &clientSize);
            if (aClient < 0) {
                throw "accept failure";
            }
            ch.handle(aClient);
            close(aClient);
        }
        close(fileDescriptor);
    });
}

void Server::stop(){
    stopLoop = 1;
	t->join(); // do not delete this!
}

Server::~Server() {
}

#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include "CLI.h"

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        SocketIO socketio(clientID);
        CLI* cli = new CLI(&socketio);
        cli->start();
    }
};

// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
    int fileDescriptor;
    volatile int stopLoop = 0;
    sockaddr_in server;
    sockaddr_in client;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */


#include "Server.h"

Server::Server(int port)throw (const char*) {
    in_procces = 0;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) throw "socket creation failed...";

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    if ((bind(socketFd, (sockaddr *) &serverAddr, sizeof(serverAddr))) != 0) throw "socket bind failed!";

    if ((listen(socketFd, 3)) != 0) throw "Listen failed!";
}

/*
void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch,this](){
        //int new_client;
        signal(SIGALRM,signal_handler);
        while(!in_procces){
            socklen_t clientSize = sizeof(clientAddr);
            alarm(2);
            int new_client = accept(socketFd,(sockaddr*)&clientAddr,&clientSize);
            if(new_client > 0){
                ch.handle(new_client);
                close(new_client);
            }
            alarm(0);
        }
        close(socketFd);
    });
}*/

void Server::start(ClientHandler& ch)throw(const char*) {
    t = new thread([&ch, this]() {
        while (!in_procces) {
            socklen_t clientSize = sizeof(clientAddr);
            int aClient = accept(socketFd, (struct sockaddr *) &clientAddr, &clientSize);
            if (aClient > 0) {
                new thread([&aClient,this,&ch](){
                ch.handle(aClient);
                close(aClient);
                });
            }
        }
        close(socketFd);
    });
}
void Server::stop(){
	t->join(); // do not delete this!
}

Server::~Server() {
}



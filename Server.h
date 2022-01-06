/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include "CLI.h"
#include <signal.h>
#include <sstream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class SocketIO: public DefaultIO {
    int clientID;
public:
    virtual string read(){
        string s="";
        char curr = 0;
        while (curr != '\n') {
            recv(clientID, &curr, 1, 0);
            s += curr;
        }
        return s;
    }
    virtual void write(string text) {

    }
    virtual void write(float f){

    }
    virtual void read(float* f){
        recv(clientID, f, sizeof(float*) , 0);
    }

    SocketIO(int id) {
        clientID = id;
    }
};

// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        SocketIO sio(clientID);
        CLI cli(&sio);
        cli.start();
        // do we need to close the sio? if we need members then we do need
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
    int socketFd;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;
    int in_procces;
	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
    static void signal_handler(int signal_num)
    {
        cout << "The interrupt signal is (" << signal_num
             << "). \n";

    }
};

#endif /* SERVER_H_ */

/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_

#include<signal.h>
#include <thread>
#include <sys/socket.h>
#include "CLI.h"
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sstream>
#include <stdlib.h>
using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class SocketIO: public DefaultIO {
    int clientID;
public:
    /*virtual string read(){
        string s="";
        char curr = 0;
        while (curr != '\n') {
            recv(clientID, &curr, 1, 0);
            s += curr;
        }
        return s;
    }*/
    string read() override {
        string input = "";
        char buffer = 0;
        recv(clientID, &buffer, 1, 0);
        while (buffer != '\n') {
            input += buffer;
            recv(clientID, &buffer, 1, 0);
        }
        return input;
    }
    virtual void write(string text) override{
        send(clientID, &text[0], text.length(), 0);
    }
    virtual void write(float f) override{
        string text = to_string(f);
        send(clientID, &text[0], text.length(), 0);
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
    struct sockaddr_in serverAddr = {0};
    struct sockaddr_in clientAddr = {0};
    bool in_procces;

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

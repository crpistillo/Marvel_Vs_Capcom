//
// Created by IgVelasco on 5/4/19.
//
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Socket.h"
#include "tools/logger/Logger.h"

using namespace std;

#define MAXPACKETSIZE 4096
#define MAXPLAYERS 2

class TCPServer
{
private:
    static void * Task(void * argv);
    int numberOfConnections;
    int port;
    int clientsSockets[MAXPLAYERS];


public:
    Socket* serverSocket;
    int sockfd, newsockfd, n, pid;
    //struct sockaddr_in serverAddress; //sockadrr_in es para protocolo IPv4
    struct sockaddr_in clientAddress;
    pthread_t serverThread;
    char msg[ MAXPACKETSIZE ];
    static string Message;

    TCPServer();
    bool setup(int port, Logger* logger);
    string receive();
    string getMessage();
    void Send(string msg);
    void detach();
    void clean();
    void initServer();
    void setSockfd(int sockfd);


};

#endif

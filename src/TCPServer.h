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
#include "data_structs.h"
#include "Team.h"
#include "tools/json/ConfigFileParser/ConfigFileParser.h"

using namespace std;

#define MAXPACKETSIZE 4096
#define MAXPLAYERS 2

class TCPServer
{
private:
	Team* team1;
	Team* team2;
    int numberOfConnections;
    int port;
    int clientsSockets[MAXPLAYERS];
    Logger* logger;

    pthread_t acceptThread;  //Identificador del thread que acepta conexiones

    pthread_t clientsThreads[MAXPLAYERS]; //Identificadores de los threads que reciven cosas de los clientes

    json config;

public:
    Socket* serverSocket;
    Socket* newSockFd;
    int n, pid;
    //struct sockaddr_in serverAddress;
    //struct sockaddr_in clientAddress;//sockadrr_in es para protocolo IPv4
    //pthread_t serverThread;
    char msg[ MAXPACKETSIZE ];
    static string Message;

    TCPServer();
    bool setup(int port, Logger* logger);
    void receive();
    void clientReceive(int socket);
    string getMessage();
    void Send(string msg);
    void detach();
    void clean();
    void initServer();
    void reportClientConnected(const struct sockaddr_in* clientAddress, socklen_t clientAddress_len, Logger* logger);
    int createAceptingThread();
    int getNumberOfConections();
    int createReceivingThreadPerClient();
    int createSendingThreadPerClient();
    int* getClientsSockets();

    void runServer();

    CharacterServer* createServerCharacter(char *character);

    void configJson(json config);
};

#endif

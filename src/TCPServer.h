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
#include "Queue/Queue.h"
#include <mutex>


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
    Socket* clientsSockets[MAXPLAYERS];
    Logger* logger;

    pthread_t acceptThread;  //Identificador del thread que acepta conexiones

    pthread_t clientsThreads[MAXPLAYERS]; //Identificadores de los threads que reciven cosas de los clientes

    json config;
    std::mutex m;

    std::mutex menuClient;


public:
    Queue<incoming_msg_t*>* incoming_msges_queue; //cola de los mensajes entrantes del cliente

    Queue<character_updater_t*>* character_updater_queue[MAXPLAYERS];
    				//colas de mensajes de escritura para cada cliente

    Queue<cliente_menu_t*>* incoming_menu_actions_queue;
    Queue<cursor_updater_t*>* cursor_updater_queue[MAXPLAYERS];

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

    void detach();
    void clean();
    void initServer();
    void reportClientConnected(const struct sockaddr_in* clientAddress, socklen_t clientAddress_len, Logger* logger);

    int getNumberOfConections();

    void runServer();

    CharacterServer* createServerCharacter(char *character, int nclient);

    void configJson(json config);

    void receiveFromClient(int clientSocket);

    void sendToClient(int clientSocket);

    Socket *getClientSocket(int i);

    bool invalidIntroAction(actions_t action);

    void runMenuPhase();
    void receiveMenuActionsFromClient(int clientSocket);
    void sendCursorUpdaterToClient(int clientSocket);
};

#endif

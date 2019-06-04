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
#include "ServerCursor.h"
#include <mutex>
#include <thread>


using namespace std;

#define MAXPACKETSIZE 4096
#define MAXPLAYERS 4

class TCPServer
{
private:

	double posPlayers[2];
    std::thread receiveFromClientThreads[MAXPLAYERS];
    std::thread sendToClientThreads[MAXPLAYERS];
	Team* team[2];
    int numberOfConnections;
    int port;
    Socket* clientsSockets[MAXPLAYERS];
    Logger* logger;
    game_instance_t server_state;

    pthread_t acceptThread;  //Identificador del thread que acepta conexiones

    pthread_t clientsThreads[MAXPLAYERS]; //Identificadores de los threads que reciven cosas de los clientes

    json config;
    ServerCursor* serverCursors[MAXPLAYERS];

    bool activeClients[MAXPLAYERS];

    void runMenuFourPlayers();
    void runMenuTwoPlayers();


    ip_status_t iplist[4];

    //MUTEXS
    std::mutex m;
    std::mutex menuClient;
    std::mutex numberOfConnections_mtx;
    std::mutex connection_mtx;
    std::mutex incoming_msg_mtx;
    std::mutex updaters_queue_mtx[MAXPLAYERS];
    std::mutex server_state_mtx;
    std::mutex teams_mtx;



public:
    Queue<incoming_msg_t*>* incoming_msges_queue; //cola de los mensajes entrantes del cliente

    Queue<character_updater_t*>* client_updater_queue[MAXPLAYERS];
    				//colas de mensajes de escritura para cada cliente

    Queue<cliente_menu_t*>* incoming_menu_actions_queue;
    Queue<cursor_updater_t*>* cursor_updater_queue[MAXPLAYERS];

    Socket* serverSocket;
    Socket* newSockFd;
    int n;

    char msg[ MAXPACKETSIZE ];

    TCPServer();
    bool setup(int port, Logger* logger, int numberOfPlayers);
    void receive();

    void detach();
    void clean();
    void initServer();
    void reportClientConnected(const struct sockaddr_in* clientAddress, socklen_t clientAddress_len, Logger* logger);

    int getNumberOfConections();

    void runServer();

    void configJson(json config);

    void receiveFromClient(int clientSocket);

    void sendToClient(int clientSocket);

    Socket *getClientSocket(int i);

    bool invalidIntroAction(actions_t action);

    void runMenuPhase();
    void receiveMenuActionsFromClient(int clientSocket);
    void sendCursorUpdaterToClient(int clientSocket);
    bool processMenuAction(cliente_menu_t *action_msg);
    int getNumberOfCharactersSelected();
    void sendUpdaters(bool finalUpdater);
    void sendSelectedCharacters();
    CharacterServer *createServerCharacterFromCursor(ServerCursor *cursor, int nclient, int characterNumber);

    int numberOfPlayers;

    void updateModel();

    void disconnectionsManager(incoming_msg_t *incoming_msg);

    bool clientIsActive(int clientSocket);

    void manageDisconnection(int clientSocket);

    void getTeams(int *teamToUpdate, int *enemyTeam, int client);

    void reconnections();

    void sendCharacterBuildersToSocket(int socketNumber);

    int getTeamNumber(int client);

    bool isIpListActive(int clientSocket);
};

#endif

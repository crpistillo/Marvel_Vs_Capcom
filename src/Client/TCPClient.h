//
// Created by IgVelasco on 5/4/19.
//
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netdb.h>
#include <vector>
#include "../Socket.h"
#include "../tools/json/ConfigFileParser/ConfigFileParser.h"
#include <pthread.h>
#include <csignal>


using namespace std;

class TCPClient
{
private:
    static TCPClient* instance;
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;
    TCPClient();



public:
    static TCPClient* getInstance();
    bool setup(string address, int port);

    void* receive(int size = 4096);
    string read();
    void exit();

    int nclient;

    Socket* socketClient;

    void run();

    void runFromBeginning();

    void runFromFight();

    void runFromMenu();

    int ancho;
    int alto;
    json config;
    int numberOfPlayers;
    int posPlayer1;
    int posPlayer2;

    void runAfterMenu() const;

    bool isPipeBroken;

    void signalHandlerClient(int signum);
};

#endif

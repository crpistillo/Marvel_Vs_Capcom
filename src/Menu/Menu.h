//
// Created by donatojp on 16/06/19.
//

#ifndef MARVEL_VS_CAPCOM_MENU_H
#define MARVEL_VS_CAPCOM_MENU_H


#include "../data_structs.h"
#include "../Queue/Queue.h"
#include "../Socket.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <sys/poll.h>


#define MAXPLAYERS 4

using namespace std;

class Menu {

protected:
    int numberOfPlayers;
    Queue<client_menu_t*>* incoming_menu_actions_queue;
    Queue<cursor_updater_t*>* cursor_updater_queue[MAXPLAYERS];

    Socket* clientsSockets[MAXPLAYERS];
    ip_status_t iplist[4];
    int* numberOfConnections;

    std::mutex runningMenuPhase_mtx;
    std::mutex connection_mtx[MAXPLAYERS];
    std::mutex incoming_msg_mtx;
    std::mutex numberOfConnections_mtx;
    bool runningMenuPhase;

public:
    Menu(int numberOfPlayers);
    void setRunningMenuPhase(bool condition);
    void receiveMenuActionsFromClient(int clientSocket);
    void sendCursorUpdaterToClient(int clientSocket);
    void sendUpdaters(bool finalUpdater);


    void runMenuPhase();
    virtual void runCorrespondingMenu() = 0;

    bool getRunningMenuPhase();

    Socket *getClientSocket(int socket);

    void setClientsSocket(Socket *pSocket[4]);

    void setIpList(ip_status_t *iplist);


    void setNumberOfConnections(int *pNumberOfConnections);
};


#endif //MARVEL_VS_CAPCOM_MENU_H

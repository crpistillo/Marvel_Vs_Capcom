//
// Created by donatojp on 16/06/19.
//

#ifndef MARVEL_VS_CAPCOM_MENU_H
#define MARVEL_VS_CAPCOM_MENU_H


#include "../data_structs.h"
#include "../Queue/Queue.h"
#include "../Socket.h"
#include "../TCPServer.h"
#include "../Constants.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <sys/poll.h>


#define MAXPLAYERS 4

using namespace std;

//No tocar esto. "declaration forward"
class TCPServer;

class Menu {

protected:
    int numberOfPlayers;
    TCPServer *server;
    Queue<client_menu_t *> *incoming_menu_actions_queue;
    Queue<cursor_updater_t *> *cursor_updater_queue[MAXPLAYERS];
    ServerCursor *serverCursors[MAXPLAYERS];

    Team* team[2];

    std::mutex menuClient;
    std::mutex runningMenuPhase_mtx;
    std::mutex connection_mtx[MAXPLAYERS];
    std::mutex incoming_msg_mtx;
    std::mutex numberOfConnections_mtx;
    std::mutex updaters_queue_mtx[MAXPLAYERS];
    bool runningMenuPhase;

public:
    Menu(int numberOfPlayers, TCPServer *pServer);
    void setRunningMenuPhase(bool condition);
    void receiveMenuActionsFromClient(int clientSocket);
    void sendCursorUpdaterToClient(int clientSocket);
    void sendUpdaters(bool finalUpdater);


    void runMenuPhase();
    virtual void runCorrespondingMenu() = 0;

    bool getRunningMenuPhase();


    bool processMenuAction(client_menu_t *action_msg);

    ServerCursor *getServerCursor(int i);

    int getNumberOfCharactersSelected();

    void sendSelectedCharacters(Constants* constants);

    int getTeamNumber(int nclient);

    CharacterServer *createServerCharacterFromCursor(ServerCursor *pCursor, int nclient, int character, Constants* constants);

    void buildTeams(Team *teams[2]);

    void reportReconnection(client_menu_t *recon_info);
};


#endif //MARVEL_VS_CAPCOM_MENU_H

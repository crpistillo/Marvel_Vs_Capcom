//
// Created by donatojp on 16/06/19.
//

#ifndef MARVEL_VS_CAPCOM_MENU_H
#define MARVEL_VS_CAPCOM_MENU_H


#include "../data_structs.h"
#include "../Queue/Queue.h"
#include <thread>
#include <mutex>

#define MAXPLAYERS 4

class Menu {

protected:
    int numberOfPlayers;
    Queue<client_menu_t*>* incoming_menu_actions_queue;
    Queue<cursor_updater_t*>* cursor_updater_queue[MAXPLAYERS];

    std::mutex runningMenuPhase_mtx;
    bool runningMenuPhase;

public:
    Menu(int numberOfPlayers);
    void setRunningMenuPhase(bool condition);
    void receiveMenuActionsFromClient(int clientSocket);
    void sendCursorUpdaterToClient(int clientSocket);
    void sendUpdaters(bool finalUpdater);


    void runMenuPhase();
    virtual void runCorrespondingMenu() = 0;

};


#endif //MARVEL_VS_CAPCOM_MENU_H

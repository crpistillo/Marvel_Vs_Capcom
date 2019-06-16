//
// Created by donatojp on 16/06/19.
//

#ifndef MARVEL_VS_CAPCOM_MENU_H
#define MARVEL_VS_CAPCOM_MENU_H


#include "../data_structs.h"
#include "../Queue/Queue.h"

#define MAXPLAYERS 4

class Menu {

private:
    int numberOfPlayers;
    Queue<client_menu_t*>* incoming_menu_actions_queue;
    Queue<cursor_updater_t*>* cursor_updater_queue[MAXPLAYERS];


public:
    Menu(int numberOfPlayers);
};


#endif //MARVEL_VS_CAPCOM_MENU_H

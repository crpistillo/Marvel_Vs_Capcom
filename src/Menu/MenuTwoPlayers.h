//
// Created by donatojp on 16/06/19.
//

#ifndef MARVEL_VS_CAPCOM_MENUTWOPLAYERS_H
#define MARVEL_VS_CAPCOM_MENUTWOPLAYERS_H


#include "Menu.h"
#include "../TCPServer.h"

class MenuTwoPlayers : public Menu {

public:
    MenuTwoPlayers(TCPServer *pServer);

    void runMenuTwoPlayers();
    void runCorrespondingMenu();
    void sendSelectedCharacters(Constants* constants);
};


#endif //MARVEL_VS_CAPCOM_MENUTWOPLAYERS_H

//
// Created by donatojp on 16/06/19.
//

#ifndef MARVEL_VS_CAPCOM_MENUFOURPLAYERS_H
#define MARVEL_VS_CAPCOM_MENUFOURPLAYERS_H


#include "Menu.h"
#include "../TCPServer.h"

class MenuFourPlayers : public Menu {

public:
    MenuFourPlayers(TCPServer *pServer);

    void runMenuFourPlayers();
    void runCorrespondingMenu();
    void sendSelectedCharacters(Constants* constants);
};


#endif //MARVEL_VS_CAPCOM_MENUFOURPLAYERS_H

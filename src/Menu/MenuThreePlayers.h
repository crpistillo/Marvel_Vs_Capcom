//
// Created by donatojp on 18/06/19.
//

#ifndef MARVEL_VS_CAPCOM_MENUTHREEPLAYERS_H
#define MARVEL_VS_CAPCOM_MENUTHREEPLAYERS_H


#include "../TCPServer.h"
#include "Menu.h"

class MenuThreePlayers : public Menu {

public:
    MenuThreePlayers(TCPServer *pServer);


    void runMenuThreePlayers();
    void runCorrespondingMenu();
    void sendSelectedCharacters(Constants* constants);
};


#endif //MARVEL_VS_CAPCOM_MENUTHREEPLAYERS_H

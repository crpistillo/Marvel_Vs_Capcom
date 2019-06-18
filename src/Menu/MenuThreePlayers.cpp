//
// Created by donatojp on 18/06/19.
//

#include "MenuThreePlayers.h"

MenuThreePlayers::MenuThreePlayers(TCPServer *pServer) : Menu(3, pServer) {
    //Team 0 por defecto es el de la persona solitaria
    this->team[0] = new Team(1);
    this->team[1] = new Team(2);
}

void MenuThreePlayers::runCorrespondingMenu() {
    runMenuThreePlayers();
}

void MenuThreePlayers::runMenuThreePlayers() {



}

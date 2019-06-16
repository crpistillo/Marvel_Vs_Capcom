//
// Created by donatojp on 16/06/19.
//

#include "MenuFourPlayers.h"

MenuFourPlayers::MenuFourPlayers(TCPServer *pServer) : Menu(4, pServer) {
}

void MenuFourPlayers::runMenuFourPlayers() {

}

void MenuFourPlayers::runCorrespondingMenu() {
    this->runMenuFourPlayers();
}

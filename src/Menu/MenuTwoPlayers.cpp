//
// Created by donatojp on 16/06/19.
//

#include "MenuTwoPlayers.h"

MenuTwoPlayers::MenuTwoPlayers(TCPServer *pServer) : Menu(2, pServer) {
}


void MenuTwoPlayers::runMenuTwoPlayers() {

}

void MenuTwoPlayers::runCorrespondingMenu() {
    this->runMenuTwoPlayers();
}

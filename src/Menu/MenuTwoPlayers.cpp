//
// Created by donatojp on 16/06/19.
//

#include "MenuTwoPlayers.h"

MenuTwoPlayers::MenuTwoPlayers(TCPServer *pServer) : Menu(2, pServer) {
}


void MenuTwoPlayers::runMenuTwoPlayers() {
    //Los dos cursores del primer cliente
    serverCursors[0] = new ServerCursor(97, 61, true);
    serverCursors[1] = new ServerCursor(449, 61, false);

    //Los dos cursores del segundo cliente
    serverCursors[2] = new ServerCursor(97, 353, true);
    serverCursors[3] = new ServerCursor(449, 353, false);

    ServerCursor *actualCursorFirstClient = serverCursors[0];
    ServerCursor *actualCursorSecondClient = serverCursors[2];

    //Procesar eventos que vengan de incoming_menu_actions_queue
    while (1) {
        client_menu_t *incoming_msg;
        if (this->incoming_menu_actions_queue->empty_queue())
            continue;
        incoming_msg = this->incoming_menu_actions_queue->get_data();

        /* Proceso el evento */

        bool validMenuAction;

        if (incoming_msg->client == 0)
            validMenuAction = actualCursorFirstClient->update(incoming_msg);
        if (incoming_msg->client == 1)
            validMenuAction = actualCursorSecondClient->update(incoming_msg);

        if (actualCursorFirstClient->getFinalSelection()) {
            actualCursorFirstClient = serverCursors[1];
            actualCursorFirstClient->setVisible(true);
        }
        if (actualCursorSecondClient->getFinalSelection()) {
            actualCursorSecondClient = serverCursors[3];
            actualCursorSecondClient->setVisible(true);
        }

        /* Solo envio información a los clientes si hubo algun cambio */
        if (validMenuAction)
            sendUpdaters(false);


        incoming_menu_actions_queue->delete_data();
        delete incoming_msg;

        /* Verifico si ya seleccionaron todos */
        int charactersSelected = this->server->getNumberOfCharactersSelected();
        if (charactersSelected == MAXPLAYERS)
            break;

    }
}

void MenuTwoPlayers::runCorrespondingMenu() {
    this->runMenuTwoPlayers();
}

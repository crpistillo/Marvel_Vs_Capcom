//
// Created by donatojp on 16/06/19.
//

#include "MenuFourPlayers.h"

MenuFourPlayers::MenuFourPlayers(TCPServer *pServer) : Menu(4, pServer) {
}

void MenuFourPlayers::runMenuFourPlayers() {
    serverCursors[0] = new ServerCursor(97, 61, true);
    serverCursors[1] = new ServerCursor(449, 61, true);
    serverCursors[2] = new ServerCursor(97, 353, true);
    serverCursors[3] = new ServerCursor(449, 353, true);
    sendUpdaters(false);

    int onlinePlayersTeamOne = 2;
    int onlinePlayersTeamTwo = 2;


    //Procesar eventos que vengan de incoming_menu_actions_queue
    while (1) {
        client_menu_t *incoming_msg;

        incoming_msg_mtx.lock();
        if (this->incoming_menu_actions_queue->empty_queue()) {
            incoming_msg_mtx.unlock();
            continue;
        }
        incoming_msg = this->incoming_menu_actions_queue->get_data();
        incoming_msg_mtx.unlock();

        if (incoming_msg->accion == DISCONNECTED_MENU) {
            if ((int) (incoming_msg->client / 2) == 0)
                onlinePlayersTeamOne--;
            else
                onlinePlayersTeamTwo--;

            if (!serverCursors[incoming_msg->client]->getFinalSelection())
                serverCursors[incoming_msg->client]->setVisible(false);
            sendUpdaters(false);
        } else if (incoming_msg->accion == RECONNECTION_MENU) {
            if ((int) (incoming_msg->client / 2) == 0)
                onlinePlayersTeamOne++;
            else
                onlinePlayersTeamTwo++;

            serverCursors[incoming_msg->client]->setVisible(true);
            sendUpdaters(false);
        } else {

            /* Proceso el evento */
            bool validMenuAction = processMenuAction(incoming_msg);

            /* Solo envio información a los clientes si hubo algun cambio */
            if (validMenuAction)
                sendUpdaters(false);
        }

        incoming_msg_mtx.lock();
        incoming_menu_actions_queue->delete_data();
        incoming_msg_mtx.unlock();

        delete incoming_msg;

        /* Verifico si ya seleccionaron todos */
        int charactersSelected = getNumberOfCharactersSelected();
        if (charactersSelected == MAXPLAYERS)
            break;

    }


}

void MenuFourPlayers::runCorrespondingMenu() {
    this->runMenuFourPlayers();
}

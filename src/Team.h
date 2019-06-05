/*
 * Team.h
 *
 *  Created on: 18 may. 2019
 *      Author: caropistillo
 */

#ifndef TEAM_H_
#define TEAM_H_

#include "CharactersServer/CharacterServer.h"
#include "Socket.h"

class Team
{
private:
	CharacterServer* currentCharacter;//me falta ver esto y otras cosas
	CharacterServer* firstCharacter; //personaje del clienteA (cada personaje tiene un cliente)
	CharacterServer* secondCharacter; //personaje del clienteB

    //numberOfClientsActive

public:
	int sizeOfTeam;

	Team(int teamSize);

    //cambia el character y el cliente del mismo

    //devuelve verdadero si ya tiene (MAX_PLAYERS/2) jugadores

    void changeCharacter(Socket** sockets);

    void update(int distance, int posContrincante, actions_t action, Socket** sockets);

    void disconnectClient();

    CharacterServer* get_currentCharacter();

    bool invalidIntroAction();

	int clientActive;

	void manageDisconection(int clientSocket);

    CharacterServer * get_firstCharacter();

    CharacterServer * get_secondCharacter();

    void setSize(int size);

    void setClientNumberToCurrentClient(Socket** sockets);

    int get_currentCharacterNumber();

    void setCharacters(CharacterServer *firstCharact, CharacterServer *secondCharact);

    void setSecondClientAsActive();

    void setFirstClientAsActive();
};




#endif /* TEAM_H_ */

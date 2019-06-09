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
	int sizeOfTeam;
	bool isChanging;
	int clientActive;
	int teamNumber;

	//numberOfClientsActive

public:
	Team(CharacterServer* firsCharact, CharacterServer* secondCharact, int teamSize, int teamNumber);

	void changePlayer(); //cambia el character y el cliente del mismo

	bool isFull(); //devuelve verdadero si ya tiene (MAX_PLAYERS/2) jugadores

	CharacterServer* typeOfCharacter(string personaje);

	void changeCharacter(Socket** sockets);
	void setCharacterToChanging();

    void update(int distance, int posContrincante, actions_t action, Socket** sockets, Box* boxContrincante);

    void changeClient();

    void makeUpdater(character_updater_t* builder);

    CharacterServer* get_currentCharacter();

    bool invalidIntroAction();


};




#endif /* TEAM_H_ */

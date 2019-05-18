/*
 * Team.h
 *
 *  Created on: 18 may. 2019
 *      Author: caropistillo
 */

#ifndef TEAM_H_
#define TEAM_H_

#include "CharactersServer/CharacterServer.h"

class Team
{
private:
	CharacterServer* currentCharacter;//me falta ver esto y otras cosas
	CharacterServer* characterA; //personaje del clienteA (cada personaje tiene un cliente)
	CharacterServer* characterB; //personaje del clienteB
	int sizeOfTeam;
	//Controls* controls; //Hacen falta?
	int changeKey;
	bool isChanging;

public:
	Team();

	void addPlayer(string personaje, TCPClient* cliente);
	void changePlayer(string personaje, TCPClient* cliente); //cambia el character y el cliente del mismo

	bool isFull(); //devuelve verdadero si ya tiene (MAX_PLAYERS/2) jugadores

	CharacterServer* typeOfCharacter(string personaje);

	void changeCharacter();
	void setCharacterToChanging();

};




#endif /* TEAM_H_ */

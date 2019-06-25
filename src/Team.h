/*
 * Team.h
 *
 *  Created on: 18 may. 2019
 *      Author: caropistillo
 */

#ifndef TEAM_H_
#define TEAM_H_

#include "CharactersServer/Characters/CharacterServer.h"
#include "Socket.h"

class Team
{
private:
	CharacterServer* currentCharacter;//me falta ver esto y otras cosas
	CharacterServer* firstCharacter; //personaje del clienteA (cada personaje tiene un cliente)
	CharacterServer* secondCharacter; //personaje del clienteB

	unsigned int roundsWon;

    //numberOfClientsActive

public:
	int sizeOfTeam;

	Team(int teamSize);

    //cambia el character y el cliente del mismo

    //devuelve verdadero si ya tiene (MAX_PLAYERS/2) jugadores

    void changeCharacter();

    void update(int distance, actions_t action, Box *boxContrincante);

    void disconnectClient();

    CharacterServer* getCurrentCharacter();

    bool invalidIntroAction();

	int clientActive;

	void manageDisconection(int clientSocket);

    CharacterServer * getFirstCharacter();

    CharacterServer * getSecondCharacter();

    void setSize(int size);

    void setClientNumberToCurrentClient();

    int get_currentCharacterNumber();

    void setCharacters(CharacterServer *firstCharact, CharacterServer *secondCharact);

    void connectClient();

    bool collidesWith(Team *enemyTeam);

    Box *getCurrentBox();

    float getSumOfLife();

    bool partnerNotDead();

    bool areBothCharactersDead();

    void resetCharacterLife();

    float lifeOfCharacterInactive();

    void incrementRoundsWon();

    int getRoundsWon();
};




#endif /* TEAM_H_ */

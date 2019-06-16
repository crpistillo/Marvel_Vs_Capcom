/*
 * Team.cpp
 *
 *  Created on: 18 may. 2019
 *      Author: caropistillo
 */

#include "Team.h"
#include "CharactersServer/SpidermanServer.h"
#include "CharactersServer/WolverineServer.h"

const int MAX_PLAYERS = 4;

Team::Team(int teamSize) {
	this->sizeOfTeam = teamSize;
}

void Team::changeCharacter() {
	int updateX = currentCharacter->getCentro();

	if (currentCharacter == firstCharacter) {
		currentCharacter = secondCharacter;
	} else {
		currentCharacter = firstCharacter;
	}
	currentCharacter->positionUpdate(&updateX);
}

void Team::update(int distance, int posContrincante, actions_t action, Box* boxContrincante) {

	if (action == DISCONNECTEDCLIENT) {
		disconnectClient();
	}

	if(action == RECONNECT){
	    connectClient();
	}

	if (action == CHANGEME && currentCharacter->currentAction == STANDING) {
        changeCharacter();
		if (sizeOfTeam == 2) {
			this->clientActive = currentCharacter->clientNumber;
			cout<<"El cliente activo es "<<clientActive<<endl;
		}
		this->currentCharacter->currentAction = MAKINGINTRO;
    }

	if (!(currentCharacter->currentAction == MAKINGINTRO));


	/// Remove comment to see contact or test
	if(currentCharacter->getColisionable()->isColliding(boxContrincante))
	    cout<< "CONTACT BIATCH" <<endl;

	else cout<<"NO"<<endl;

	currentCharacter->update(distance, posContrincante, action, boxContrincante);
}

void Team::disconnectClient() {

	if (currentCharacter == firstCharacter) {
		this->clientActive = secondCharacter->clientNumber;
	}

	else {
		this->clientActive = firstCharacter->clientNumber;
	}
	this->sizeOfTeam--;
}

CharacterServer* Team::get_currentCharacter() {
	return this->currentCharacter;
}

bool Team::invalidIntroAction() {
	actions_t action = currentCharacter->currentAction;

	return ((action == JUMPINGLEFT) || (action == JUMPINGRIGHT)
			|| (action == JUMPINGVERTICAL) || (action == DUCK)
			|| (action == MOVINGRIGHT) || (action == MOVINGLEFT)
			|| (action == WALKBACK) || (action == PUNCH) || (action == KICK) || (action == PUNCHDOWN)
		      || (action == KICKDOWN) || (action == BLOCK));
}

CharacterServer * Team::get_firstCharacter() {
	return this->firstCharacter;

}

CharacterServer * Team::get_secondCharacter() {
	return this->secondCharacter;
}

void Team::setSize(int size) {
	this->sizeOfTeam = size;
}

void Team::setClientNumberToCurrentClient() {
	this->clientActive = currentCharacter->clientNumber;
}

int Team::get_currentCharacterNumber() {
    if(currentCharacter == firstCharacter)
        return 0;
    else
        return 1;

}

void Team::setCharacters(CharacterServer *firstCharact, CharacterServer *secondCharact) {
    this->currentCharacter = firstCharact;  //es el que esta jugando ahora
    this->firstCharacter = firstCharact;
    this->secondCharacter = secondCharact;
    clientActive = currentCharacter->clientNumber;

}

void Team::setSecondClientAsActive() {
    clientActive = secondCharacter->clientNumber;

}

void Team::setFirstClientAsActive() {
    clientActive = firstCharacter->clientNumber;
}

void Team::connectClient() {
    this->sizeOfTeam++;
    this->setClientNumberToCurrentClient();
}



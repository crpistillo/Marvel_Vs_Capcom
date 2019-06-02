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

Team::Team(CharacterServer* firstCharact, CharacterServer* secondCharact,
		int teamSize, int teamNumber) {
	this->currentCharacter = firstCharact;  //es el que esta jugando ahora
	this->firstCharacter = firstCharact;
	this->secondCharacter = secondCharact;
	this->sizeOfTeam = teamSize;
	this->teamNumber = teamNumber;
	isChanging = false;
	clientActive = currentCharacter->clientNumber;

}

void Team::changeCharacter(Socket** sockets) {
	sockets[currentCharacter->clientNumber]->receivingFromClient = false;
	int updateX = currentCharacter->getCentro();

	if (currentCharacter == firstCharacter) {
		currentCharacter = secondCharacter;
	} else {
		currentCharacter = firstCharacter;
	}
	currentCharacter->positionUpdate(&updateX);

	sockets[currentCharacter->clientNumber]->receivingFromClient = true;

}

void Team::update(int distance, int posContrincante, actions_t action,
		Socket** sockets) {

	if (action == DISCONNECTEDCLIENT) {
		disconnectClient();
	}

	if (action == CHANGEME && currentCharacter->currentAction == STANDING) {
		changeCharacter(sockets);
		if (sizeOfTeam == 2) {
			this->clientActive = currentCharacter->clientNumber;
		}
		this->currentCharacter->currentAction = MAKINGINTRO;
		isChanging = true;
	}

	if (!(currentCharacter->currentAction == MAKINGINTRO))
		isChanging = false;

	currentCharacter->update(distance, posContrincante, action);
}

void Team::disconnectClient() {

	if (currentCharacter == firstCharacter) {
		this->clientActive = secondCharacter->clientNumber;
	}

	else {
		this->clientActive = firstCharacter->clientNumber;
	}
	this->sizeOfTeam = 1;
}

CharacterServer* Team::get_currentCharacter() {
	return this->currentCharacter;
}

bool Team::invalidIntroAction() {
	actions_t action = currentCharacter->currentAction;

	return ((action == JUMPINGLEFT) || (action == JUMPINGRIGHT)
			|| (action == JUMPINGVERTICAL) || (action == DUCK)
			|| (action == MOVINGRIGHT) || (action == MOVINGLEFT)
			|| (action == WALKBACK));
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

void Team::setClientNumberToCurrentClient(Socket** sockets) {
	sockets[currentCharacter->clientNumber]->receivingFromClient = false;
	this->clientActive = currentCharacter->clientNumber;
	sockets[currentCharacter->clientNumber]->receivingFromClient = true;
}

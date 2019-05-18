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

Team::Team()
{
	this->characterA = NULL;
	this->characterB = NULL;
	this->sizeOfTeam = 0;
	isChanging = false;
}

void Team::changePlayer(string personaje, TCPClient* cliente)
{
	CharacterServer* character = this->typeOfCharacter(personaje);

	if(this->characterA->get_Client()==NULL)
	{
		this->characterA = character; //no se si asi esta bien y el typeOfCharacter
									  //tampoco se si se hace asi
		this->characterA->set_Client(cliente);

	}
	else
	{
		this->characterB = character;
		this->characterB->set_Client(cliente);
	}
}

void Team::addPlayer(string personaje, TCPClient* cliente)
{
	this->sizeOfTeam+=1;

	this->changePlayer(personaje, cliente);

}

CharacterServer* Team::typeOfCharacter(string personaje)
{

	CharacterServer* character;
	if(personaje == "Spiderman")
	{
		character = (SpidermanServer*)character;
	}
	else if(personaje == "Wolverine")
	{
		character = (WolverineServer*)character;
	}
	return character;
}


bool Team::isFull()
{
	return (this->sizeOfTeam==MAX_PLAYERS/2);
}


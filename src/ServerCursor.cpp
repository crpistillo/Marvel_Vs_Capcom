#include "ServerCursor.h"

using namespace std;

ServerCursor::ServerCursor(int posX, int posY, bool isVisible){
	this->posX = posX;
	this->posY = posY;
	this->visible = isVisible;
	this->finalSelection = false;
	this->characterSelected = NOT_VALID;
}

bool ServerCursor::moveRight(){

	if(this->finalSelection)
		return false;

	if (posX == 97){
		posX = 449;
		return true;	//Si el movimiento es válido, devuelvo true
	}

	return false;		//Si el movimiento es invalido, devuelvo false
}


bool ServerCursor::moveLeft(){

	if(this->finalSelection)
		return false;

	if (posX == 449){
		posX = 97;

		return true;	//Si el movimiento es válido, devuelvo true
	}


	return false;		//Si el movimiento es invalido, devuelvo false
}

bool ServerCursor::selectCharacter(){
	if(this->finalSelection)
		return false;

	if(this->posX == 97){
		this->characterSelected = SPIDERMAN;
		this->finalSelection = true;
	}
	else if(this->posX == 449){
		this->characterSelected = WOLVERINE;
		this->finalSelection = true;
	}
	return true;

}

bool ServerCursor::update(client_menu_t* action_msg){

	switch(action_msg->accion){
	case RIGHT:
		return this->moveRight();

	case LEFT:
		return this->moveLeft();

	case ENTER:
		return this->selectCharacter();

	case SELECT:
		return this->selectCharacter();

	default:
		return false;
	}

}

bool ServerCursor::getFinalSelection(){
	return this->finalSelection;
}

character_number_t ServerCursor::getCharacterSelected(){
	return this->characterSelected;
}

void ServerCursor::makeMenuUpdater(cursor_updater_t* updater){
	updater->posX = this->posX;
	updater->posY = this->posY;
	updater->finalSelection = this->finalSelection;
	updater->visible = this->visible;
}

void ServerCursor::setVisible(bool condition){
	this->visible = condition;
}


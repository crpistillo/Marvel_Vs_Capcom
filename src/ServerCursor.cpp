#include "ServerCursor.h"

using namespace std;

ServerCursor::ServerCursor(int posX, int posY){
	this->posX = posX;
	this->posY = posY;
	this->finalSelection = false;
	this->characterSelected = NOT_VALID;
}

void ServerCursor::moveRight(){

	if (posX == 97){
		posX = 449;
		cout << "Nueva posicion del cursor: " + to_string(posX) + "\n";
		return;
	}

	cout << "Cursor no puede moverse \n";
	return;
}


void ServerCursor::moveLeft(){
	if (posX == 449){
		posX = 97;
		cout << "Nueva posicion del cursor: " + to_string(posX) + "\n";
		return;
	}

	cout << "Cursor no puede moverse \n";
	return;
}

void ServerCursor::update(cliente_menu_t* action_msg){

	switch(action_msg->accion){
	case RIGHT:
		this->moveRight();
		break;
	case LEFT:
		this->moveLeft();
		break;

	case ENTER:
		return;

	default:
		return;
	}


	return;
}

void ServerCursor::makeMenuUpdater(cursor_updater_t* updater){
	updater->posX = this->posX;
	updater->posY = this->posY;
	updater->finalSelection = this->finalSelection;
}



#include "ServerCursor.h"

using namespace std;

ServerCursor::ServerCursor(int posX, int posY){
	this->posX = posX;
	this->posY = posY;
	this->finalSelection = false;
	this->characterSelected = NOT_VALID;
}

void ServerCursor::moveRight(){
	return;
}


void ServerCursor::moveLeft(){
	return;
}



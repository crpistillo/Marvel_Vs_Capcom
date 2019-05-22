#include "ClientCursor.h"


using namespace std;

unsigned int numberOfCursors = 0;
string PATH_CURSOR_CLIENT_1 = "images/menu/cliente1.png";
string PATH_CURSOR_CLIENT_2 = "images/menu/cliente2.png";
string PATH_CURSOR_CLIENT_3 = "images/menu/cliente3.png";
string PATH_CURSOR_CLIENT_4 = "images/menu/cliente4.png";

ClientCursor::ClientCursor(int posX, int posY, SDL_Renderer *renderer){
	this->posX = posX;
	this->posY = posY;
	this->finalSelection = false;

	if(numberOfCursors == 0){
		this->cursorTexture.loadFromFile(PATH_CURSOR_CLIENT_1, renderer);
		this->posY = 61;
	}
	else if(numberOfCursors == 1){
		this->cursorTexture.loadFromFile(PATH_CURSOR_CLIENT_2, renderer);
		this->posY = 61;
	}
	else if(numberOfCursors == 2){
		this->cursorTexture.loadFromFile(PATH_CURSOR_CLIENT_3, renderer);
		this->posY = 353;
	}
	else if(numberOfCursors == 3){
		this->cursorTexture.loadFromFile(PATH_CURSOR_CLIENT_4, renderer);
		this->posY = 353;
	}

	numberOfCursors++;
}


void ClientCursor::update(cursor_updater_t* updater){
	this->posX = updater->posX;
	//this->posY = updater->posY; Creo que esta posicion es fija, no necesita actualizarse
	this->finalSelection = updater->finalSelection;
}

void ClientCursor::render(SDL_Renderer* mRenderer){
	cursorTexture.render(posX, posY, 254, 221, mRenderer);
}


#include "ClientCursor.h"


using namespace std;

unsigned int numberOfCursors = 0;
string PATH_CURSOR_CLIENT_1 = "images/menu/cliente1.png";
string PATH_CURSOR_CLIENT_2 = "images/menu/cliente2.png";
string PATH_CURSOR_CLIENT_3 = "images/menu/cliente3.png";
string PATH_CURSOR_CLIENT_4 = "images/menu/cliente4.png";

ClientCursor::ClientCursor(int posX, int posY, SDL_Renderer *renderer, bool isVisible) {
	this->posX = posX;
	this->posY = posY;
	this->finalSelection = false;
	this->clientRepresenting = -1;

	if(numberOfCursors == 0)
		this->cursorTexture.loadFromFile(PATH_CURSOR_CLIENT_1, renderer);
	else if(numberOfCursors == 1)
		this->cursorTexture.loadFromFile(PATH_CURSOR_CLIENT_2, renderer);
	else if(numberOfCursors == 2)
		this->cursorTexture.loadFromFile(PATH_CURSOR_CLIENT_3, renderer);
	else if(numberOfCursors == 3)
		this->cursorTexture.loadFromFile(PATH_CURSOR_CLIENT_4, renderer);

	numberOfCursors++;
}


void ClientCursor::update(cursor_updater_t* updater){
	this->posX = updater->posX;
	this->clientRepresenting = updater->cliente;
	//this->posY = updater->posY; Creo que esta posicion es fija, no necesita actualizarse
	this->finalSelection = updater->finalSelection;
}

void ClientCursor::render(SDL_Renderer* mRenderer){
    if(!visible)
        return;

	if(this->finalSelection)
		switch(this->clientRepresenting){
		case 0:
			this->cursorTexture.loadFromFile("images/menu/cliente1v.png", mRenderer);
			break;
		case 1:
			this->cursorTexture.loadFromFile("images/menu/cliente2v.png", mRenderer);
			break;
		case 2:
			this->cursorTexture.loadFromFile("images/menu/cliente3v.png", mRenderer);
			break;
		case 3:
			this->cursorTexture.loadFromFile("images/menu/cliente4v.png", mRenderer);
			break;
		}


	cursorTexture.render(posX, posY, 254, 221, mRenderer);
}


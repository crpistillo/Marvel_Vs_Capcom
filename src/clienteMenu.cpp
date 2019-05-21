/*
 * clienteMenu.cpp
 *
 *  Created on: 20 may. 2019
 *      Author: german
 */


#include "clienteMenu.h"

using namespace std;

clienteMenu::clienteMenu(int posX, int team) {
    this->posX = posX;
    this->team = team;
}


/*void clienteMenu::moveLeft(SDL_Renderer *renderer) {
	posX = 97;

}


void clienteMenu::moveRight(SDL_Renderer *renderer) {
    posX = 449;
}*/


void clienteMenu::render(SDL_Renderer *mRenderer, int posX) {
	int posY;
	if (team == 1) posY = 61;
	else posY = 353;
    m_Texture.render(posX, posY, 254, 221, mRenderer);
}


void clienteMenu::load(SDL_Renderer *renderer, string path) {
    m_Texture.loadFromFile(path, renderer);
}


void clienteMenu::free() {
    m_Texture.free();
}



//
// Created by amaherok on 4/6/19.
//

#include "Player.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

Player::Player(Character *first, Character *second, Controls* controls,Logger* logger) {
	logger->log("Inicializacion de personajes para jugador.", DEBUG);
    this->controls = controls;
    currentCharacter = first;
    firstCharacter = first;
    secondCharacter = second;
    isChanging = false;

    changeKey = controls->changeKey;
    logger->log("Inicializacion de controles para jugador.", DEBUG);
    firstCharacter->setControls(controls);
    secondCharacter->setControls(controls);
}



void Player::update(SDL_Renderer *renderer, int distance, int posContrincante,Logger* logger) {

    InputManager* inputManager = InputManager::getInstance();
    logger->log("Detecta boton para cambio de personaje en Player.", DEBUG);
    if(inputManager->isKeyDown(changeKey) && !isChanging && !(currentCharacter->isJumpingVertical || currentCharacter->isJumpingRight || currentCharacter->isJumpingLeft)){
        changeCharacter();
        setCharacterToChanging();
        isChanging = true;
    }
    if(!currentCharacter->isMakingIntro)
        isChanging = false;
    currentCharacter->update(renderer, distance, posContrincante,logger);
}

void Player::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante,Logger* logger) {
	logger->log("Renderizado de personaje - Render.", DEBUG);
	currentCharacter->render(mRenderer, camX, camY, posContrincante,logger);
}

void Player::free() {
    delete firstCharacter;
    delete secondCharacter;
}

void Player::changeCharacter() {
    /*En lugar de madarle al otro personaje la posicion en x del sprite,
     * se le envia el centro (justo la posicion del personaje donde debe estar)*/
	int updateX = currentCharacter->getCentro();

    if(currentCharacter == firstCharacter) {
    	currentCharacter = secondCharacter;
    }
    else {
        currentCharacter = firstCharacter;
    }
    currentCharacter->positionUpdate(&updateX);
    //animacion

}

void Player::setCharacterToChanging(){
	currentCharacter->startIntro();
}

void Player::loads(SDL_Renderer *pRenderer) {
    firstCharacter->load(pRenderer);
    secondCharacter->load(pRenderer);
}

int Player::getPosX() {
    return currentCharacter->getPosX();
}


int Player::getWidth() {
    return currentCharacter->getWidth();
}

int Player::getSobrante() {
    return currentCharacter->getSobrante();
}

int Player::getCentro() {
    return currentCharacter->getCentro();
}

Character* Player::getCurrentCharacter()
{
	return this->currentCharacter;
}

Player::~Player() {
    delete controls;
    delete firstCharacter;
    delete secondCharacter;

}

int Player::getZIndex(){
	return this->currentCharacter->getZIndex();

}



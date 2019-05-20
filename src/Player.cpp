//
// Created by amaherok on 4/6/19.
//

#include "Player.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

Player::Player(CharacterClient *first, CharacterClient *second) {
	Logger* logger = Logger::getInstance();
	logger->log("Inicializacion de personajes para jugador.", DEBUG);
	currentCharacter = first;
    firstCharacter = first;
    secondCharacter = second;
    isChanging = false;

}


void Player::update(character_updater_t* updater) {


    if(updater->action == CHANGEME){
        changeCharacter();  //send change character
        setCharacterToChanging();
        isChanging = true;
    }
    currentCharacter->update(updater);
	Logger* logger = Logger::getInstance();
    InputManager* inputManager = InputManager::getInstance();
    logger->log("Detecta boton para cambio de personaje en Player.", DEBUG);

    if(!(currentCharacter->currentAction == MAKINGINTRO))
        isChanging = false;
   // currentCharacter->load(renderer, distance, posContrincante);
}


void Player::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) {
	Logger* logger = Logger::getInstance();
	logger->log("Renderizado de personaje - Render.", DEBUG);
	currentCharacter->render(mRenderer, camX, camY, posContrincante);
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

void Player::loads(SDL_Renderer *pRenderer, int posContrincante) {
    firstCharacter->load(pRenderer, posContrincante);
    secondCharacter->load(pRenderer, posContrincante);
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

CharacterClient* Player::getCurrentCharacter()
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

void Player::load(SDL_Renderer *pRenderer, int posContrincante) {
    currentCharacter->load(pRenderer, posContrincante);
}




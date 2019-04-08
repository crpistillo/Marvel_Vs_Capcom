//
// Created by amaherok on 4/6/19.
//

#include "Player.h"

Player::Player(Character *first, Character *second, Controls* controls) {
    currentCharacter = first;
    firstCharacter = first;
    secondCharacter = second;
    isChanging = false;

    changeKey = controls->changeKey;
    firstCharacter->setControls(controls);
    secondCharacter->setControls(controls);
}



void Player::update(SDL_Renderer *renderer, int distance, int posContrincante) {
    InputManager* inputManager = InputManager::getInstance();
    if(inputManager->isKeyDown(changeKey) && !isChanging){
        changeCharacter();
        setCharacterToChanging();
        isChanging = true;
        std::cout << "hola";
    }
    if(!currentCharacter->isMakingIntro)
        isChanging = false;
    currentCharacter->update(renderer, distance, posContrincante);
}

void Player::render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante) {
    currentCharacter->render(mRenderer, camX, camY, posContrincante);
}

void Player::free() {
    firstCharacter->free();
    secondCharacter->free();
}

void Player::changeCharacter() {
    int updateX = currentCharacter->getPosX();
    int updateY = currentCharacter->getPosY();

    if(currentCharacter == firstCharacter)
        currentCharacter = secondCharacter;
    else
        currentCharacter = firstCharacter;
    currentCharacter->positionUpdate(&updateX, &updateY);
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
    currentCharacter->getPosX();
}



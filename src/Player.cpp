//
// Created by amaherok on 4/6/19.
//

#include "Player.h"

Player::Player(Character *first, Character *second) {
    currentCharacter = first;
    firstCharacter = first;
    secondCharacter = second;
}



void Player::update(SDL_Renderer *renderer, int distance, int posContrincante) {
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
    if(currentCharacter == firstCharacter)
        currentCharacter = secondCharacter;
    else
        currentCharacter = firstCharacter;

    //hay que setear las mismas posiciones y tecla de cambio
    //animacion

}

void Player::loads(SDL_Renderer *pRenderer) {
    firstCharacter->load(pRenderer);
    secondCharacter->load(pRenderer);
}

int Player::getPosX() {
    currentCharacter->getPosX();
}



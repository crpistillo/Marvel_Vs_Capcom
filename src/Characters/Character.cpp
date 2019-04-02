/*
 * Character.cpp
 *
 *  Created on: 2 abr. 2019
 *      Author: sebastian
 */

#include "Character.h"

// Protected
Character::Character(
	int mPosX,
	int mPosY,
	int currentWalkingLeftSprite,
	int currentWalkingRightSprite,
	int currentStandingSprite,
	int currentJumpingSprite,
	bool isLookingLeft,
	string name,
	SDL_Keycode upKey,
	SDL_Keycode downKey,
	SDL_Keycode rightKey,
	SDL_Keycode leftKey
) {
	this->mPosX = mPosX;
	this->mPosY = mPosY;
	this->mVelX = 0;
	this->mVelY = 0;
	this->currentWalkingLeftSprite = currentWalkingLeftSprite;
	this->currentWalkingRightSprite = currentWalkingRightSprite;
	this->currentStandingSprite = currentStandingSprite;
	this->currentJumpingSprite = currentJumpingSprite;
	this->isLookingLeft = isLookingLeft;
	this->isStanding = true;
	this->upKey = upKey;
	this->downKey = downKey;
	this->rightKey = rightKey;
	this->leftKey = leftKey;
}

// Public:
void Character::handleEvent(SDL_Event &e, SDL_Renderer *renderer, int distance) {
    //If a key was pressed
    if (e.type == SDL_KEYDOWN) {
        isStanding = false;
        SDL_Keycode key = e.key.keysym.sym;
        if(key == this->upKey) {
        	this->jump(renderer);
        }
        if(key == this->downKey) {
        	this->renderDuckSprite(renderer);
		}
        if(key == this->leftKey) {
        	this->moveLeft(renderer, distance);
		}
        if(key == this->rightKey) {
        	this->moveRight(renderer, distance);
		}
    }
        //If a key was released
    else if (e.type == SDL_KEYUP) {
        isStanding = true;
        SDL_Keycode key = e.key.keysym.sym;
		if(key == this->upKey) {
			this->renderStandSprite(renderer);
		}
		if(key == this->downKey) {
			this->renderStandSprite(renderer);
		}
		if(key == this->leftKey) {
			this->renderStandSprite(renderer);
		}
		if(key == this->rightKey) {
			this->renderStandSprite(renderer);
		}
    }
}

void Character::render(SDL_Renderer *mRenderer, int camX, int camY) {
    if (isStanding)
        renderStandSprite(mRenderer);
    m_Texture.render(mPosX - camX, mPosY - camY, mRenderer);
}

void Character::free() {
    m_Texture.free();
}

int Character::getPosX() {
    //243 es una corrección debida a que el ancho de la imagen es mas grande que el personaje
	return mPosX;
}

int Character::getPosY() {
    return mPosY;
}

Character::~Character() {
	this->free();
}



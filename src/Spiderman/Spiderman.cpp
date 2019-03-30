//
// Created by amaherok on 3/28/19.
//

#include "Spiderman.h"

using namespace std;

const int LEVEL_WIDTH = 695;
const int LEVEL_HEIGHT = 600;

const int INITIAL_POS_X = 400;
const int INITIAL_POST_Y = 470;

const int FIRST_WALKING_SPRITE = 170;
const int LAST_WALKING_SPRITE = 180;
int currentWalkingLeftSprite = 170;
int currentWalkingRightSprite = 170;

const int FIRST_JUMPING_SPRITE = 189;
const int LAST_JUMPING_SPRITE = 196;
int currentJumpingSprite = 189;

Spiderman::Spiderman() {
    mPosX = INITIAL_POS_X;
    mPosY = INITIAL_POST_Y;
    mVelX = 0;
    mVelY = 0;
    isLookingLeft = false;
}

void Spiderman::handleEvent(SDL_Event &e, SDL_Renderer *renderer) {
	const Uint8* SDL_GetKeyboardState(int* numkeys);
    //If a key was pressed
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
            	this->jump(renderer);
                break;
            case SDLK_DOWN:
            	m_Texture.loadFromFile("images/MVC2_SpiderMan_432.png", renderer);
                break;
            case SDLK_LEFT:
            	this->moveLeft(renderer);
                break;
            case SDLK_RIGHT:
            	this->moveRight(renderer);
                break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
            	//TODO
            	mPosY = INITIAL_POST_Y;
            	currentJumpingSprite = FIRST_JUMPING_SPRITE;
            	this->renderStandSprite(renderer);
                break;
            case SDLK_DOWN:
            	this->renderStandSprite(renderer);
                break;
            case SDLK_LEFT:
            	this->renderStandSprite(renderer);
            	currentWalkingRightSprite = 170;
            	currentWalkingLeftSprite = 170;
                break;
            case SDLK_RIGHT:
            	this->renderStandSprite(renderer);
            	currentWalkingRightSprite = 170;
				currentWalkingLeftSprite = 170;
                break;
        }
    }
}

void Spiderman::renderStandSprite(SDL_Renderer *renderer) {
	if(isLookingLeft) {
		m_Texture.loadFromFile("images/spiderman_stand_left.png", renderer);
	} else {
		m_Texture.loadFromFile("images/spiderman_stand_right.png", renderer);
	}
}

void Spiderman::moveLeft(SDL_Renderer *renderer) {
	isLookingLeft = true;

	if(mPosX - DOT_VEL <= 0) {
		return;
	}

	if(currentWalkingLeftSprite < FIRST_WALKING_SPRITE) {
		currentWalkingLeftSprite = LAST_WALKING_SPRITE;
	}
	string imagePath = "images/spiderman_walking_left/MVC2_SpiderMan_" + to_string(currentWalkingLeftSprite) + ".png";
	m_Texture.loadFromFile(imagePath, renderer);
	--currentWalkingLeftSprite;

	//If the dot is inside the screen move
	if(mPosX - DOT_VEL > 0) {
		mPosX -= DOT_VEL;
	}
}

void Spiderman::moveRight(SDL_Renderer *renderer) {
	isLookingLeft = false;

	if(mPosX + DOT_VEL >= LEVEL_WIDTH) {
		return;
	}

	if(currentWalkingRightSprite > LAST_WALKING_SPRITE) {
		currentWalkingRightSprite = FIRST_WALKING_SPRITE;
	}
	string imagePath = "images/spiderman_walking_right/MVC2_SpiderMan_" + to_string(currentWalkingRightSprite) + ".png";
	m_Texture.loadFromFile(imagePath, renderer);
	++currentWalkingRightSprite;

	//If the dot is inside the screen move
	if( mPosX + DOT_VEL < LEVEL_WIDTH ) {
		mPosX += DOT_VEL;
	}
}

void Spiderman::jump(SDL_Renderer *renderer) {
	if(currentJumpingSprite > LAST_JUMPING_SPRITE) {
		currentJumpingSprite = FIRST_JUMPING_SPRITE;
		mPosY = INITIAL_POST_Y;
	}

	string imagePath = "images/spiderman_jumping/MVC2_SpiderMan_" + to_string(currentJumpingSprite) + ".png";
	m_Texture.loadFromFile(imagePath, renderer);

	if(currentJumpingSprite < 193) {
		mPosY -= DOT_VEL;
	}
	if(currentJumpingSprite > 193) {
		mPosY += DOT_VEL;
	}

	++currentJumpingSprite;
}

void Spiderman::move() {
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > LEVEL_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > LEVEL_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Spiderman::render(SDL_Renderer *mRenderer) {
    m_Texture.render(mPosX, mPosY, mRenderer);
}

void Spiderman::spidermanLoad(SDL_Renderer *renderer) {
    m_Texture.loadFromFile("images/spiderman_stand_right.png", renderer);
}

void Spiderman::free() {
    m_Texture.free();
}



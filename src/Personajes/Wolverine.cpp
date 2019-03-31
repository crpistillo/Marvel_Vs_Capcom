//
// Created by amaherok on 3/31/19.
//

#include "Wolverine.h"

using namespace std;

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

const int INITIAL_POS_X = 0;
const int INITIAL_POST_Y = 270;

const int FIRST_WALKING_SPRITE = 39;
const int LAST_WALKING_SPRITE = 54;

const int FIRST_JUMPING_SPRITE = 189;
const int LAST_JUMPING_SPRITE = 196;

Wolverine::Wolverine() {
    mPosX = INITIAL_POS_X;
    mPosY = INITIAL_POST_Y;
    mVelX = 0;
    mVelY = 0;
    currentWalkingLeftSprite = LAST_WALKING_SPRITE;
    currentWalkingRightSprite = FIRST_WALKING_SPRITE;
    currentJumpingSprite = 189;
    isLookingLeft = false;
}


void Wolverine::handleEvent(SDL_Event &e, SDL_Renderer *renderer) {
    //If a key was pressed
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                this->jump(renderer);
                break;
            case SDLK_s:
                m_Texture.loadFromFile("images/MVC2_Wolverine_432.png", renderer);
                break;
            case SDLK_a:
                this->moveLeft(renderer);
                break;
            case SDLK_d:
                this->moveRight(renderer);
                break;
        }
    }
        //If a key was released
    else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                mPosY = INITIAL_POST_Y;
                currentJumpingSprite = FIRST_JUMPING_SPRITE;
                this->renderStandSprite(renderer);
                break;
            case SDLK_s:
                this->renderStandSprite(renderer);
                break;
            case SDLK_a:
                this->renderStandSprite(renderer);
                currentWalkingRightSprite = currentWalkingLeftSprite = FIRST_WALKING_SPRITE;
                break;
            case SDLK_d:
                this->renderStandSprite(renderer);
                currentWalkingRightSprite = currentWalkingLeftSprite = FIRST_WALKING_SPRITE;
                break;
        }
    }
}

void Wolverine::renderStandSprite(SDL_Renderer *renderer) {
    if (isLookingLeft) {
        m_Texture.loadFromFile("images/wolverine_stand_left.png", renderer);
    } else {
        m_Texture.loadFromFile("images/wolverine_stand_right.png", renderer);
    }
}

void Wolverine::moveLeft(SDL_Renderer *renderer) {
    isLookingLeft = true;

    if (mPosX - DOT_VEL <= 0) {
        return;
    }

    if (currentWalkingLeftSprite < FIRST_WALKING_SPRITE) {
        currentWalkingLeftSprite = LAST_WALKING_SPRITE;
    }
    string imagePath = "images/wolverine_walking_left/MVC2_Wolverine_" + to_string(currentWalkingLeftSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);
    --currentWalkingLeftSprite;

    //If the dot is inside the screen move
    /*if(mPosX - DOT_VEL > 0) {
        mPosX -= DOT_VEL;
    }*/

    //Move the dot left or right
    mPosX -= DOT_VEL;

    //If the dot went too far to the left or right
    if ((mPosX < -3000) || (mPosX + DOT_WIDTH > LEVEL_WIDTH)) {
        //Move back
        mPosX += DOT_VEL;
    }
}

void Wolverine::moveRight(SDL_Renderer *renderer) {
    isLookingLeft = false;

    if (mPosX + DOT_VEL >= LEVEL_WIDTH) {
        return;
    }

    if (currentWalkingRightSprite > LAST_WALKING_SPRITE) {
        currentWalkingRightSprite = FIRST_WALKING_SPRITE;
    }
    string imagePath = "images/wolverine_walking_right/MVC2_Wolverine_" + to_string(currentWalkingRightSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);
    ++currentWalkingRightSprite;

    //If the dot is inside the screen move
    /*if( mPosX + DOT_VEL < LEVEL_WIDTH ) {
        mPosX += DOT_VEL;
    }*/

    //Move the dot left or right
    mPosX += DOT_VEL;

    //If the dot went too far to the left or right
    if ((mPosX < 0) || (mPosX + DOT_WIDTH > LEVEL_WIDTH)) {
        //Move back
        mPosX -= DOT_VEL;
    }

}

void Wolverine::jump(SDL_Renderer *renderer) {
    if (currentJumpingSprite > LAST_JUMPING_SPRITE) {
        currentJumpingSprite = FIRST_JUMPING_SPRITE;
        mPosY = INITIAL_POST_Y;
    }

    string imagePath = "images/wolverine_jumping/MVC2_Wolverine_" + to_string(currentJumpingSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);

    if (currentJumpingSprite < 193) {
        mPosY -= DOT_VEL;
    }
    if (currentJumpingSprite > 193) {
        mPosY += DOT_VEL;
    }

    ++currentJumpingSprite;
}

void Wolverine::render(SDL_Renderer *mRenderer, int camX, int camY) {
    m_Texture.render(mPosX - camX, mPosY - camY, mRenderer);
}

void Wolverine::wolverineLoad(SDL_Renderer *renderer) {
    m_Texture.loadFromFile("images/wolverine_stand_right.png", renderer);
}

void Wolverine::free() {
    m_Texture.free();
}


int Wolverine::getPosX() {
    return mPosX;
}

int Wolverine::getPosY() {
    return mPosY;
}
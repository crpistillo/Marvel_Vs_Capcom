//
// Created by amaherok on 3/28/19.
//

#include "Spiderman.h"

using namespace std;

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

const int INITIAL_POS_X = 350;
const int INITIAL_POST_Y = 270;

const int LAST_STANDING_SPRITE = 167;
const int FIRST_STANDING_SPRITE = 159;

const int FIRST_WALKING_SPRITE = 170;
const int LAST_WALKING_SPRITE = 180;

const int FIRST_JUMPING_SPRITE = 189;
const int LAST_JUMPING_SPRITE = 196;

Spiderman::Spiderman() {
    mPosX = INITIAL_POS_X;
    mPosY = INITIAL_POST_Y;
    mVelX = 0;
    mVelY = 0;
    currentWalkingLeftSprite = 170;
    currentStandingSprite = 158;
    currentWalkingRightSprite = 170;
    currentJumpingSprite = 189;
    isLookingLeft = false;
    isStanding = true;
}

void Spiderman::handleEvent(SDL_Event &e, SDL_Renderer *renderer, int distancia) {
    //If a key was pressed
    if (e.type == SDL_KEYDOWN) {
        isStanding = false;
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                this->jump(renderer);
                break;
            case SDLK_DOWN:
                m_Texture.loadFromFile("images/MVC2_SpiderMan_432.png", renderer);
                break;
            case SDLK_LEFT:
                this->moveLeft(renderer, distancia);
                break;
            case SDLK_RIGHT:
                this->moveRight(renderer, distancia);
                break;
        }
    }
        //If a key was released
    else if (e.type == SDL_KEYUP) {
        isStanding = true;
        switch (e.key.keysym.sym) {
            case SDLK_UP:
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
    if (isLookingLeft) {
        m_Texture.loadFromFile("images/spiderman_stand_left.png", renderer);
    } else {
        if (currentStandingSprite > LAST_STANDING_SPRITE)
            currentStandingSprite = FIRST_STANDING_SPRITE;
        string imagePath = "images/spiderman_standing_right/MVC2_SpiderMan_" + to_string(currentStandingSprite) + ".png";
        m_Texture.loadFromFile(imagePath, renderer);
    }
}

void Spiderman::moveLeft(SDL_Renderer *renderer, int distancia) {
    isLookingLeft = true;

    //Puse -320 en lugar de 0 porque la imagen del personaje es mas ancha que él.
    //La distancia es la distancia entre personajes.
    if ((mPosX - DOT_VEL <= -320) || (distancia < (-600))) {
        return;
    }

    if (currentWalkingLeftSprite < FIRST_WALKING_SPRITE) {
        currentWalkingLeftSprite = LAST_WALKING_SPRITE;
    }
    string imagePath = "images/spiderman_walking_left/MVC2_SpiderMan_" + to_string(currentWalkingLeftSprite) + ".png";
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

void Spiderman::moveRight(SDL_Renderer *renderer, int distancia) {
    isLookingLeft = false;

    if ((mPosX + DOT_VEL >= (LEVEL_WIDTH - 420)) || (distancia > 600)) {
        return;
    }

    if (currentWalkingRightSprite > LAST_WALKING_SPRITE) {
        currentWalkingRightSprite = FIRST_WALKING_SPRITE;
    }
    string imagePath = "images/spiderman_walking_right/MVC2_SpiderMan_" + to_string(currentWalkingRightSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);
    ++currentWalkingRightSprite;

    //If the dot is inside the screen move
    /*if( mPosX + DOT_VEL < LEVEL_WIDTH ) {
        mPosX += DOT_VEL;
    }*/

    //Move the dot left or right
    mPosX += DOT_VEL;

    //If the dot went too far to the left or right
    if ((mPosX < -320) || (mPosX + DOT_WIDTH > LEVEL_WIDTH)) {
        //Move back
        mPosX -= DOT_VEL;
    }

}

void Spiderman::jump(SDL_Renderer *renderer) {
    if (currentJumpingSprite > LAST_JUMPING_SPRITE) {
        currentJumpingSprite = FIRST_JUMPING_SPRITE;
        mPosY = INITIAL_POST_Y;
    }

    string imagePath = "images/spiderman_jumping/MVC2_SpiderMan_" + to_string(currentJumpingSprite) + ".png";
    m_Texture.loadFromFile(imagePath, renderer);

    if (currentJumpingSprite < 193) {
        mPosY -= DOT_VEL;
    }
    if (currentJumpingSprite > 193) {
        mPosY += DOT_VEL;
    }

    ++currentJumpingSprite;
}

void Spiderman::render(SDL_Renderer *mRenderer, int camX, int camY) {
    if (isStanding)
        renderStandSprite(mRenderer);
    m_Texture.render(mPosX - camX, mPosY - camY, mRenderer);
}

void Spiderman::spidermanLoad(SDL_Renderer *renderer) {
    m_Texture.loadFromFile("images/spiderman_stand_right/MVC2_SpiderMan_159.png", renderer);
}

void Spiderman::free() {
    m_Texture.free();
}


int Spiderman::getPosX() {
    return mPosX;
}

int Spiderman::getPosY() {
    return mPosY;
}

void Spiderman::cambioDeSprites() {
    if (currentStandingSprite <= LAST_STANDING_SPRITE)
        currentStandingSprite++;
}

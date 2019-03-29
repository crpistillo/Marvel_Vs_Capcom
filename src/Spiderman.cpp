//
// Created by amaherok on 3/28/19.
//

#include "Spiderman.h"

const int LEVEL_WIDTH = 695;
const int LEVEL_HEIGHT = 600;

Spiderman::Spiderman() {
    mPosX = 400;
    mPosY = 470;
    mVelX = 0;
    mVelY = 0;
}


void Spiderman::handleEvent(SDL_Event &e, SDL_Renderer *renderer) {
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        //Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY -= DOT_VEL;
                break;
            case SDLK_DOWN:
            	m_Texture.loadFromFile("images/MVC2_SpiderMan_432.png", renderer);
                break;
            case SDLK_LEFT:
                mVelX -= DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX += DOT_VEL;
                break;
        }
    }
        //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        //Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY += DOT_VEL;
                break;
            case SDLK_DOWN:
            	m_Texture.loadFromFile("images/SpiderMan397.png", renderer);
                break;
            case SDLK_LEFT:
                mVelX += DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX -= DOT_VEL;
                break;
        }
    }
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
    m_Texture.loadFromFile("images/SpiderMan397.png", renderer);
}

void Spiderman::free() {
    m_Texture.free();
}



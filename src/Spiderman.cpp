//
// Created by amaherok on 3/28/19.
//

#include "Spiderman.h"

Spiderman::Spiderman() {
    mPosX = 0;
    mPosY = 0;
    mVelX = 0;
    mVelY = 0;
}


void Spiderman::handleEvent(SDL_Event &e) {
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        //Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY -= DOT_VEL;
                break;
            case SDLK_DOWN:
                mVelY += DOT_VEL;
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
                mVelY -= DOT_VEL;
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

    //Move the dot up or down
    mPosY += mVelY;
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



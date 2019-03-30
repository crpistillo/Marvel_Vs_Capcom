//
// Created by amaherok on 3/28/19.
//

#ifndef MARVEL_VS_CAPCOM_SPIDERMAN_H
#define MARVEL_VS_CAPCOM_SPIDERMAN_H


#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"

class Spiderman {
public:
    //The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    //Maximum axis velocity of the dot
    static const int DOT_VEL = 10;

    //Initializes the variables
    Spiderman();
    void free();

    void spidermanLoad(SDL_Renderer* renderer);
    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event &e, SDL_Renderer* renderer);

    //Moves the dot
    void move();

    //Shows the dot on the screen
    void render(SDL_Renderer* mRenderer);

private:
    //The X and Y offsets of the dot
    int mPosX, mPosY;

    //The velocity of the dot
    int mVelX, mVelY;

    Texture m_Texture;
};


#endif //MARVEL_VS_CAPCOM_SPIDERMAN_H

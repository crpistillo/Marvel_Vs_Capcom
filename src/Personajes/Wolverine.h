//
// Created by amaherok on 3/31/19.
//

#ifndef MARVEL_VS_CAPCOM_WOLVERINE_H
#define MARVEL_VS_CAPCOM_WOLVERINE_H


#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include "../Texture/Texture.h"

class Wolverine {
public:
    //The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    //Maximum axis velocity of the dot
    static const int DOT_VEL = 10;

    //Initializes the variables
    Wolverine();
    void free();

    void wolverineLoad(SDL_Renderer* renderer);
    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event &e, SDL_Renderer* renderer);

    //Moves the dot
    void move();

    //Shows the dot on the screen
    void render(SDL_Renderer* mRenderer, int camX, int camY);

    int getPosX();
    int getPosY();

private:
    //The X and Y offsets of the dot
    int mPosX, mPosY;
    //The velocity of the dot
    int mVelX, mVelY;

    Texture m_Texture;

    int currentWalkingLeftSprite;
    int currentWalkingRightSprite;
    int currentJumpingSprite;
    bool isLookingLeft;

    void renderStandSprite(SDL_Renderer *renderer);
    void moveRight(SDL_Renderer *renderer);
    void moveLeft(SDL_Renderer *renderer);
    void jump(SDL_Renderer *renderer);
};


#endif //MARVEL_VS_CAPCOM_WOLVERINE_H

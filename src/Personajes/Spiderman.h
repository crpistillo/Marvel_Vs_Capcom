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
    void handleEvent(SDL_Event &e, SDL_Renderer* renderer, int distancia);

    //Moves the dot
    void move();

    //Shows the dot on the screen
    void render(SDL_Renderer* mRenderer, int camX, int camY);

    int getPosX();
    int getPosY();

    void cambioDeSprites();

private:
    //The X and Y offsets of the dot
    int mPosX, mPosY;
    //The velocity of the dot
    int mVelX, mVelY;

    Texture m_Texture;
    int currentWalkingLeftSprite;
    int currentStandingSprite;
    int currentWalkingRightSprite;
    int currentJumpingSprite;
    bool isLookingLeft;
    bool isStanding;

    void renderStandSprite(SDL_Renderer *renderer);
    void moveRight(SDL_Renderer *renderer, int distancia);
    void moveLeft(SDL_Renderer *renderer, int distancia);
    void jump(SDL_Renderer *renderer);
};


#endif //MARVEL_VS_CAPCOM_SPIDERMAN_H

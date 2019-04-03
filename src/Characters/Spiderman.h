//
// Created by amaherok on 3/28/19.
//

#ifndef MARVEL_VS_CAPCOM_SPIDERMAN_H
#define MARVEL_VS_CAPCOM_SPIDERMAN_H


#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "./Character.h"

class Spiderman final: public Character {
public:
    //Initializes the variables
    Spiderman(string name);
    void load(SDL_Renderer* renderer);

private:
    void resetSpriteVariables();
    void renderStandSprite(SDL_Renderer *renderer);
    void renderDuckSprite(SDL_Renderer *renderer);
    void moveRight(SDL_Renderer *renderer, int distance, int posContrincante);
    void moveLeft(SDL_Renderer *renderer, int distance, int posContrincante);
    void jump(SDL_Renderer *renderer);
    void updateStand();
    void animacionRight(SDL_Renderer *renderer);
    void animacionLeft(SDL_Renderer *renderer);
};


#endif //MARVEL_VS_CAPCOM_SPIDERMAN_H

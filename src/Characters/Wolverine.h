//
// Created by amaherok on 3/31/19.
//

#ifndef MARVEL_VS_CAPCOM_WOLVERINE_H
#define MARVEL_VS_CAPCOM_WOLVERINE_H


#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "./Character.h"

class Wolverine final: public Character {
public:
    //Initializes the variables
	Wolverine(string name);
    void load(SDL_Renderer* renderer);

private:
    void resetSpriteVariables();
    void renderStandSprite(SDL_Renderer *renderer);
    void renderDuckSprite(SDL_Renderer *renderer);
    void moveRight(SDL_Renderer *renderer, int distance);
    void moveLeft(SDL_Renderer *renderer, int distance);
    void jump(SDL_Renderer *renderer);
    void updateStand();
};


#endif //MARVEL_VS_CAPCOM_WOLVERINE_H

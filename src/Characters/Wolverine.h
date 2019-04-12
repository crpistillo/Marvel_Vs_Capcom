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
	Wolverine(string name, int PosX, bool secondaryColor);
    void load(SDL_Renderer* renderer);
	static const int CHARACTER_WIDTH = 157;
	static const int CHARACTER_HEIGHT = 20;

	static const int SOBRANTE = 502;

private:
    void resetSpriteVariables();
    void renderStandSprite(SDL_Renderer *renderer);
    void loadStandSprite(SDL_Renderer *renderer, char position);
    void renderDuckSprite(SDL_Renderer *renderer);
    void moveRight(SDL_Renderer *renderer, int distance, int posContrincante);
    void moveLeft(SDL_Renderer *renderer, int distance, int posContrincante);
    void jump(SDL_Renderer *renderer);
    void jumpRight(SDL_Renderer *renderer);
    void jumpLeft(SDL_Renderer *renderer);
    void updateStand();
    void animacionRight(SDL_Renderer *renderer);
    void animacionLeft(SDL_Renderer *renderer);
    void repositionHeightAfterJump(char direction);
    void makeIntro(SDL_Renderer* renderer);
};


#endif //MARVEL_VS_CAPCOM_WOLVERINE_H

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
	Wolverine(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho, int anchoPantalla);
    void load(SDL_Renderer *renderer, int posContrincante);
	//static const int CHARACTER_WIDTH = 157;
	//static const int CHARACTER_HEIGHT = 20;

	//static const int SOBRANTE = 502;

private:
    void resetSpriteVariables();
    void stand();
    void renderDuckSprite();
    void moveRight(int distance, int posContrincante);
    void moveLeft(int distance, int posContrincante);
    void jumpVertical();
    void jumpRight();
    void jumpLeft();
    void updateStand();
    void walkingSpriteUpdate();

    void makeIntro();


    void jump(int *currentSprite, int lastSprite);
};


#endif //MARVEL_VS_CAPCOM_WOLVERINE_H

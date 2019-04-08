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
    Spiderman(string name, int PosX);
    ~Spiderman();
    void load(SDL_Renderer* renderer);

	/*Es la dimension del personaje sin contar el fondo magenta. Se mide "a mano" con un
	 * editor de texto, asegurándose de, primero, haber llevado la imágen (en stand by)
	 * a la resolucion especificada en el renderQuad*/
	static const int CHARACTER_WIDTH = 198;
	static const int CHARACTER_HEIGHT = 20;

	/*Es el ancho entre el inicio de la imagen (a izquierda) hasta el personaje.
	 * Tomarlas mismas precauciones para medir que con la dimension del personaje*/
	static const int SOBRANTE = 437;

private:

	ImageLoader loader;

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


#endif //MARVEL_VS_CAPCOM_SPIDERMAN_H

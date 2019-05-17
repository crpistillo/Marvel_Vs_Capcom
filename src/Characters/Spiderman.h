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
    Spiderman(int PosX, bool secondaryColor, int width, int height, int sobrante, int ancho, int anchoPantalla);
    ~Spiderman();
    void load(SDL_Renderer *renderer, int posContrincante);

	/*Es la dimension del personaje sin contar el fondo magenta. Se mide "a mano" con un
	 * editor de texto, asegurándose de, primero, haber llevado la imágen (en stand by)
	 * a la resolucion especificada en el renderQuad*/
	//static const int CHARACTER_WIDTH = 198;
	//static const int CHARACTER_HEIGHT = 20;

	/*Es el ancho entre el inicio de la imagen (a izquierda) hasta el personaje.
	 * Tomarlas mismas precauciones para medir que con la dimension del personaje*/
	//static const int SOBRANTE = 437;

private:


    void moveRight(int distance, int posContrincante);
    void moveLeft(int distance, int posContrincante);

    void walkingSpriteUpdate();

};


#endif //MARVEL_VS_CAPCOM_SPIDERMAN_H

/*
 * Layer.h
 *
 *  Created on: 9 abr. 2019
 *      Author: caropistillo
 */

#ifndef LAYER_H_
#define LAYER_H_

#include "Texture/Texture.h"
#include "Player.h"
#include<SDL2/SDL.h>

const int LEVEL_WIDTH = 3200;
const int LEVEL_HEIGHT = 600;

class Layer
{
    public:

		//Inicializa las variales
		Layer(int layerWidth, int layerHeight, float layerVel, int posX);

		//Mueve el layer
		void move();

		//Muestra el layer en la pantalla, relativo a la camara
		void render( int camX, int camY, SDL_Renderer *mRenderer, Texture* gTexture);

		//Controla el movimiento del layer segun si la camara se movio a la derecha o izquierda
		void update(int centerBefore, int centerLater);

		//Position accessors
		double getPosX();
		double getPosY();
		double getVel();
		void changeVel(float vel);

    private:
		int layerWidth;
		int layerHeight;
		double layerVel;

		//The X and Y offsets of the dot
		double mPosX, mPosY;

		//The velocity of the dot
		double mVelX, mVelY;

};


#endif /* LAYER_H_ */

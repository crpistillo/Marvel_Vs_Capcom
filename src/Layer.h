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

		//Initializes the variables
		Layer(int layerWidth, int layerHeight, float layerVel, int posX);

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e);

		//Moves the dot
		void move();

		//Shows the dot on the screen relative to the camera

		void render( int camX, int camY, SDL_Renderer *mRenderer, Texture* gTexture);

		void update(int centerBefore, int centerLater);

		//Position accessors
		float getPosX();
		float getPosY();
		float getVel();

    private:
		int layerWidth;
		int layerHeight;
		float layerVel;

		//The X and Y offsets of the dot
		float mPosX, mPosY;

		//The velocity of the dot
		float mVelX, mVelY;

		int changeKey;

};


#endif /* LAYER_H_ */

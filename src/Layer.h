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

class Layer
{
    public:

		//Initializes the variables
		Layer(int layerWidth, int layerHeight, float layerVel, int posX);

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e);

		//Moves the dot
		void move(int levelWidth, int level_height);

		//Shows the dot on the screen relative to the camera

		void render( int camX, int camY, SDL_Renderer *mRenderer, Texture* gTexture);

		void update(int centerBefore, int centerLater);

		//Position accessors
		float getPosX();
		float getPosY();

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

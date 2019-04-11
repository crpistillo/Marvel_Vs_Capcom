/*
 * Layer.cpp
 *
 *  Created on: 9 abr. 2019
 *      Author: caropistillo
 */

#include "Layer.h"
#include "InputManager.h"
#include "Controls/Controls.h"



Layer::Layer(int layerWidth, int layerHeight, float layerVel, int posX)
{
	this->layerWidth = layerWidth;
	this->layerHeight = layerHeight;
	this->layerVel = layerVel;
	this-> mPosX = posX;
	this-> mPosY = 0;
    this->mVelX = 0;
    this->mVelY = 0;

}

void Layer::update(int centerBefore, int centerLater)
{

		if(centerLater > centerBefore)
		{
			mVelX -= this->layerVel;
			this->move();
			mVelX=0;

		}
		else if (centerLater < centerBefore)
		{
			mVelX += this->layerVel;
			this->move();
			mVelX=0;
		}



	//si se mueve a la derecha


}

void Layer::handleEvent( SDL_Event& e)
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_LEFT: mVelX -= this->layerVel; break;
            case SDLK_RIGHT: mVelX += this->layerVel; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += this->layerVel; break;
            case SDLK_DOWN: mVelY -= this->layerVel; break;
            case SDLK_LEFT: mVelX += this->layerVel; break;
            case SDLK_RIGHT: mVelX -= this->layerVel; break;
        }
    }
}

void Layer::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + this->layerWidth > LEVEL_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + this->layerHeight > LEVEL_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Layer::render( int camX, int camY, SDL_Renderer* mRenderer, Texture* gTexture)
{
    //Show the dot relative to the camera
	gTexture->render( mPosX - camX, mPosY - camY, layerWidth+100, layerHeight, mRenderer);
}

float Layer::getPosX()
{
	return mPosX;
}

float Layer::getPosY()
{
	return mPosY;
}

float Layer::getVel()
{
	return mVelX;
}



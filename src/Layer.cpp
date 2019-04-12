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
		//Si la camara se esta moviendo para la derecha
		if(centerLater > centerBefore)
		{
			mVelX -= this->layerVel;
			this->move();
			mVelX=0;

		}

		//Si la camara se esta moviendo hacia la izquierda
		else if (centerLater < centerBefore)
		{
			mVelX += this->layerVel;
			this->move();
			mVelX=0;
		}
}


void Layer::move()
{
    //Mueve el layer a la derecha o a la izquierda
    mPosX += mVelX;

    //Si el layer se fue muy a la derecha o a la izquierda
    if( ( mPosX < 0 ) || ( mPosX + this->layerWidth > LEVEL_WIDTH ) )
    {
        //Lo mueve para atras
        mPosX -= mVelX;
    }
}

void Layer::render( int camX, int camY, SDL_Renderer* mRenderer, Texture* gTexture)
{
    //Muestra el layer relativo a la camara
	gTexture->render( mPosX - camX, mPosY - camY, layerWidth+100, layerHeight, mRenderer);
}

double Layer::getPosX()
{
	return mPosX;
}

double Layer::getPosY()
{
	return mPosY;
}

double Layer::getVel()
{
	return this->layerVel;
}

void Layer::changeVel(float vel)
{
	this->layerVel = vel;
}



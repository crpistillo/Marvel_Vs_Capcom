/*
 * Parallax.cpp
 *
 *  Created on: 11 abr. 2019
 *      Author: caropistillo
 */

#include "Parallax.h"

Parallax::Parallax(Layer** middleGround, Layer** backGround, SDL_Rect* camera, int* centerBefore, int* centerLater)
{
	this->middleGround = middleGround;
	this->backGround = backGround;
	this->camera = camera;
	this->centerBefore = centerBefore;
	this->centerLater = centerLater;
	this->screenWidth = 800;
	this->levelWidth= 3200;
}

bool Parallax::playersAreMoving(Player* player1, Player* player2)
{
	return player1->getCurrentCharacter()->isMoving() && player2->getCurrentCharacter()->isMoving();
}

void Parallax::adjustLayerVelocity(Player** player1, Player** player2)
{
	if(this->playersAreMoving(*player1, *player2))
	{
		(*middleGround)->changeVel(3.33);
	   	(*backGround)->changeVel(6.715);
	}
    else
    {
        (*middleGround)->changeVel(1.665);
        (*backGround)->changeVel(3.3575);
	}
}

void Parallax::centerCamera(Player** player1, Player** player2)
{
	camera->x = ((*player1)->getCentro() + (*player2)->getCentro()) / 2 - screenWidth / 2;
	camera->y = 0;
}

void Parallax::setCenterBefore()
{
	*(this->centerBefore) = this->screenWidth/2 + camera->x;
}

void Parallax::keepCameraAndCenterInBounds()
{
	if ((this->camera->x) < 2)
	{
		this->camera->x = 2;
		(*(this->centerBefore)) = this->screenWidth/2;
		(*(this->middleGround))->adjustPosX(0.400009);
		(*(this->backGround))->adjustPosX(0.914982);
	}

	if ((this->camera->x) > this->levelWidth - (this->camera->w))
	{
		this->camera->x = 3200 - this->camera->w;
		(*(this->centerBefore)) = this->levelWidth - this->screenWidth/2;
		(*(this->middleGround))->adjustPosX(799.6);
		(*(this->backGround))->adjustPosX(1599.09);
	}
}

void Parallax::updateLayers()
{
	if(*(this->centerLater)!=-1000)// && (this->camera->x > 2 && this->camera->x < this->levelWidth-this->screenWidth))
	{
		(*(this->middleGround))->update(*(this->centerBefore), *(this->centerLater));
		(*(this->backGround))->update(*(this->centerBefore), *(this->centerLater));
	}
}

void Parallax::setCenterLater()
{
	*(this->centerLater) = this->screenWidth/2 + camera->x;
}

void Parallax::doParallax(Player** player1, Player** player2)
{
	this->adjustLayerVelocity(player1,player2);

	this->centerCamera(player1, player2);

	this->setCenterBefore();

	this->keepCameraAndCenterInBounds();

	this->updateLayers();

	this->setCenterLater();
}







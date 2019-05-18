/*
 * Parallax.cpp
 *
 *  Created on: 11 abr. 2019
 *      Author: caropistillo
 */

#include "Parallax.h"


const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";


Parallax::Parallax(Layer** middleGround, Layer** backGround, SDL_Rect* camera, int* centerBefore, int* centerLater, Logger* logger, int screenWidth)
{
	logger->log("Definicion middleGround Parallax.", DEBUG);
	this->middleGround = middleGround;
	logger->log("Definicion backGround Parallax.", DEBUG);
	this->backGround = backGround;
	logger->log("Definicion camera Parallax.", DEBUG);
	this->camera = camera;
	logger->log("Definicion centerBefore Parallax.", DEBUG);
	this->centerBefore = centerBefore;
	logger->log("Definicion centerLater Parallax.", DEBUG);
	this->centerLater = centerLater;
	logger->log("Definicion screenWidth Parallax.", DEBUG);
	this->screenWidth = screenWidth;
	logger->log("Definicion levelWidth Parallax.", DEBUG);
	this->levelWidth= 3200;
}

bool Parallax::playersAreMoving(Player* player1, Player* player2,Logger* logger)
{
	logger->log("Marca personajes en movimiento.", DEBUG);
    return player1->getCurrentCharacter()->isMoving() && player2->getCurrentCharacter()->isMoving();
}


void Parallax::adjustLayerVelocity(Player** player1, Player** player2,Logger* logger)
{
	if(this->playersAreMoving(*player1, *player2,logger))
	{
		logger->log("Def. velocidad de movimiento parallax con jg. en mov.", DEBUG);
		(*middleGround)->changeVel(3.33);
	   	(*backGround)->changeVel(6.715);
	}
    else
    {
    	logger->log("Def. velocidad de movimiento parallax con jg. estaticos.", DEBUG);
        (*middleGround)->changeVel(1.665);
        (*backGround)->changeVel(3.3575);
	}
}

void Parallax::centerCamera(Player** player1, Player** player2,Logger* logger)
{
	logger->log("Parallax - Centra camara.", DEBUG);
	camera->x = ((*player1)->getCentro() + (*player2)->getCentro()) / 2 - screenWidth / 2;
	camera->y = 0;
}

void Parallax::setCenterBefore(Logger* logger)
{
	logger->log("Parallax - Define centro.", DEBUG);
	*(this->centerBefore) = this->screenWidth/2 + camera->x;
}

void Parallax::keepCameraAndCenterInBounds(Logger* logger)
{
	logger->log("Parallax - ajusta posicion segun camara.", DEBUG);
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

void Parallax::updateLayers(Logger* logger)
{
	if(*(this->centerLater)!=-1000)// && (this->camera->x > 2 && this->camera->x < this->levelWidth-this->screenWidth))
	{
		(*(this->middleGround))->update(*(this->centerBefore), *(this->centerLater));
		(*(this->backGround))->update(*(this->centerBefore), *(this->centerLater));
	}
}

void Parallax::setCenterLater(Logger* logger)
{
	*(this->centerLater) = this->screenWidth/2 + camera->x;
}

void Parallax::doParallax(Player** player1, Player** player2,Logger* logger)
{
	logger->log("Parallax - llama Ajusta velocidad del layer.", DEBUG);
	this->adjustLayerVelocity(player1,player2,logger);

	logger->log("Parallax - llama Centra camara.", DEBUG);
	this->centerCamera(player1, player2,logger);

	logger->log("Parallax - llama Ubica el centro anterior.", DEBUG);
	this->setCenterBefore(logger);

	logger->log("Parallax - llama Mantiene la camara y el centro.", DEBUG);
	this->keepCameraAndCenterInBounds(logger);

	logger->log("Parallax - llama Actualiza layers.", DEBUG);
	this->updateLayers(logger);

	logger->log("Parallax - llama Setea el centro del layer.", DEBUG);
	this->setCenterLater(logger);
}







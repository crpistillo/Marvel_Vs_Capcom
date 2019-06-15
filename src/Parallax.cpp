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
	(*middleGround)->changeVel(1.665);
	(*backGround)->changeVel(3.3575);
	bool layersCentered = false;
}

bool Parallax::playersAreMoving(Player* player1, Player* player2,Logger* logger)
{
	logger->log("Marca personajes en movimiento.", DEBUG);
    return player1->getCurrentCharacter()->isMoving() && player2->getCurrentCharacter()->isMoving();
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
	if ((this->camera->x) < 9) //antes era 2
	{
		this->camera->x = 9; //antes era 2
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
	logger->log("Parallax - llama Centra camara.", DEBUG);
	this->centerCamera(player1, player2,logger);

	if (!layersCentered)
	{
		this->centerLayers(player1, player2);
		this->layersCentered = true;
	}

	logger->log("Parallax - llama Ubica el centro anterior.", DEBUG);
	this->setCenterBefore(logger);

	logger->log("Parallax - llama Mantiene la camara y el centro.", DEBUG);
	this->keepCameraAndCenterInBounds(logger);

	logger->log("Parallax - llama Actualiza layers.", DEBUG);
	this->updateLayers(logger);

	logger->log("Parallax - llama Setea el centro del layer.", DEBUG);
	this->setCenterLater(logger);
}

void Parallax::centerLayers(Player** player1, Player** player2)
{
	double posPlayer1 = (*player1)->getPosX();
	double posPlayer2 = (*player2)->getPosX();

 	double center = (posPlayer1+posPlayer2)/2 + 534;
	double dif = center-1578;

 	cout<<"La pos de player 1 es: "<<posPlayer1+534<<endl;
	cout<<"La pos del player 2 es: "<<posPlayer2+534<<endl;
	cout<<"El centro es "<<center<<endl;
	(*(this->middleGround))->adjustPosX(399.06+dif*0.3218);//0,32359, //0,32156774 /0,322539
	(*(this->backGround))->adjustPosX(799.09+dif*0.6667);//910


 }







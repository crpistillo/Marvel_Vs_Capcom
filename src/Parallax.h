/*
 * Parallax.h
 *
 *  Created on: 11 abr. 2019
 *      Author: caropistillo
 */

#ifndef PARALLAX_H_
#define PARALLAX_H_

#include "Layer.h"
#include "tools/logger/Logger.h"

const int MIDDLE_GROUND_VEL = 3.33;
const int BACK_GROUND_VEL = 6.66667;

class Parallax
{
private:
	Layer** middleGround;
	Layer** backGround;
	SDL_Rect* camera;
	int* centerBefore;
	int* centerLater;
	int screenWidth;
	int levelWidth;

public:
	Parallax(Layer** middleGround, Layer** backGround, SDL_Rect* camera, int* centerBefore, int* centerLater, Logger* logger, int screenWidth);

	bool playersAreMoving(Player* player1, Player* player2,Logger* logger);

	void adjustLayerVelocity(Player** player1, Player** player2,Logger* logger);

	//centra la camara entre los personajes
	void centerCamera(Player** player1, Player** player2,Logger* logger);

	void setCenterBefore(Logger* logger);

	//Mantengo la camara y el centro dentro de los límites
	void keepCameraAndCenterInBounds(Logger* logger);

	void updateLayers(Logger* logger);

	void setCenterLater(Logger* logger);

	void doParallax(Player** player1, Player** player2,Logger* logger);


};



#endif /* PARALLAX_H_ */

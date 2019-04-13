/*
 * Parallax.h
 *
 *  Created on: 11 abr. 2019
 *      Author: caropistillo
 */

#ifndef PARALLAX_H_
#define PARALLAX_H_

#include "Layer.h"

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
	Parallax(Layer** middleGround, Layer** backGround, SDL_Rect* camera, int* centerBefore, int* centerLater);

	bool playersAreMoving(Player* player1, Player* player2);

	void adjustLayerVelocity(Player** player1, Player** player2);

	//centra la camara entre los personajes
	void centerCamera(Player** player1, Player** player2);

	void setCenterBefore();

	//Mantengo la camara y el centro dentro de los límites
	void keepCameraAndCenterInBounds();

	void updateLayers();

	void setCenterLater();

	void doParallax(Player** player1, Player** player2);


};



#endif /* PARALLAX_H_ */

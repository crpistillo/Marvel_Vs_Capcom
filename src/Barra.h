/*
 * Barra.h
 *
 *  Created on: 21 jun. 2019
 *      Author: caropistillo
 */

#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL.h"

#ifndef BARRA_H_
#define BARRA_H_

class Barra {
private:
	int posX;
	int posY;
	int width;
	int height;
	float health = 1;
	SDL_Color backColor;
	SDL_Color frontColor;
	int hx;
	int hw;
    Uint8 r_value;
    Uint8 g_value;
    Uint8 b_value;

	//SDL_Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

public:
	void render(SDL_Renderer* m_Renderer, SDL_Color frontColor, SDL_Color backColor);
	SDL_Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	Barra();
	void update(float life);
	void setBarra(bool left, bool isCurrent);
	virtual ~Barra();
};

#endif /* BARRA_H_ */

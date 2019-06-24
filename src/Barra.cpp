/*
 * Barra.cpp
 *
 *  Created on: 21 jun. 2019
 *      Author: caropistillo
 */

#include "Barra.h"
#include<iostream>

const int WIDTH_BAR = 259;
const int HEIGHT_BAR = 17;
const int POS_X_LEFT = 88;
const int POS_X_RIGHT = 453;
const int POS_Y_CURRENT = 88;
const int POS_Y_SECONDARY = 61;

const Uint8 SECONDARY_RED = 255;
const Uint8 SECONDARY_GREEN = 200;
const Uint8 SECONDARY_BLUE = 000;

Barra::Barra() {
	this->width = WIDTH_BAR;
	this->height = HEIGHT_BAR;
	this->health = 1;


}

Barra::~Barra() {
	// TODO Auto-generated destructor stub
}

void Barra::render(SDL_Renderer* m_Renderer, SDL_Color frontColor, SDL_Color backColor)
{
	health = health > 1.f ? 1.f : health < 0.f ? 0.f : health;
	SDL_Color old;
	SDL_GetRenderDrawColor(m_Renderer, &old.r, &old.g, &old.g, &old.a);
	SDL_Rect bgrect = { posX, posY, width, height };
	SDL_SetRenderDrawColor(m_Renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderFillRect(m_Renderer, &bgrect);
	SDL_SetRenderDrawColor(m_Renderer, frontColor.r, frontColor.g, frontColor.b, frontColor.a);

	SDL_Rect fgrect = { hx, posY, hw, height };
	SDL_RenderFillRect(m_Renderer, &fgrect);
	SDL_SetRenderDrawColor(m_Renderer, old.r, old.g, old.b, old.a);
}

SDL_Color Barra::color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
		SDL_Color col = {r,g,b,a};
		return col;
}

void Barra::update(float life)
{
	this->health = life/100;
	this->hw = (int) ((float) width * health);

}

void Barra::setBarra(bool left, bool isCurrent)
{
	this->hw = (int) ((float) width * health);

	if(left)
	{
		this->posX = POS_X_LEFT;
		this->hx = this->posX;
		if(isCurrent)
			this->posY = POS_Y_CURRENT;
		else this->posY = POS_Y_SECONDARY;
	}
	else
	{
		this->posX = POS_X_RIGHT;
		this->hx = posX + (width - hw);
		if(isCurrent)
			this->posY = POS_Y_CURRENT;
		else this->posY = POS_Y_SECONDARY;
	}
}


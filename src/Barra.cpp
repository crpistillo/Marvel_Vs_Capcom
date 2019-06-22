/*
 * Barra.cpp
 *
 *  Created on: 21 jun. 2019
 *      Author: caropistillo
 */

#include "Barra.h"
#include<iostream>

const int WIDTH_BAR = 279;
const int HEIGHT_BAR = 17;
const int POS_X_LEFT = 88;
const int POS_X_RIGHT = 434;
const int POS_Y = 74;

Barra::Barra() {
	this->width = WIDTH_BAR;
	this->height = HEIGHT_BAR;
	this->health = 1;
	frontColor = color(255,255,0,1);
	backColor= color(167,000,0,0.57);
	this->posY = POS_Y;

}

Barra::~Barra() {
	// TODO Auto-generated destructor stub
}

void Barra::render(SDL_Renderer* m_Renderer)
{
	health = health > 1.f ? 1.f : health < 0.f ? 0.f : health;
	std::cout<<"Health es: "<<health<<std::endl;
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

void Barra::setBarra(bool left)
{
	this->hw = (int) ((float) width * health);

	if(left)
	{
		this->posX = POS_X_LEFT;
		this->hx = this->posX;
	}
	else
	{
		this->posX = POS_X_RIGHT;
		this->hx = posX + (width - hw);
	}
}


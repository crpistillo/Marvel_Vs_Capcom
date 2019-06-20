/*
 * Banner.cpp
 *
 *  Created on: 20 jun. 2019
 *      Author: caropistillo
 */

#include "Banner.h"


#include "Banner.h"
#include "InputManager.h"
#include "Controls/Controls.h"



Banner::Banner(int BannerWidth, int BannerHeight, float BannerVel, int posX)
{


}

void Banner::free()
{
	texture.free();
}

void Banner::render(SDL_Renderer* renderer)
{
	texture.render(0,0,800,600,renderer);
}

void Banner::load(SDL_Renderer* renderer)
{
	texture.loadFromFile("images/barras/banner.png",renderer);
}

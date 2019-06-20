/*
 * Banner.h
 *
 *  Created on: 20 jun. 2019
 *      Author: caropistillo
 */

#include "Texture/Texture.h"
#include "Player.h"
#include <SDL2/SDL.h>

#ifndef BANNER_H_
#define BANNER_H_


class Banner: public Renderizable
{
    public:

		//Inicializa las variales
		Banner(int BannerWidth, int BannerHeight, float BannerVel, int posX);
		void free();
		void render(SDL_Renderer* renderer);
		void load(SDL_Renderer* renderer);

    private:
		Texture texture;



};



#endif /* BANNER_H_ */

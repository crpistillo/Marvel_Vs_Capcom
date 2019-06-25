//
// Created by donatojp on 25/06/19.
//

#ifndef MARVEL_VS_CAPCOM_ROUNDCOUNTER_H
#define MARVEL_VS_CAPCOM_ROUNDCOUNTER_H

#include "Texture/Texture.h"

class RoundCounter {
public:
    int counter;
    int posX;
    RoundCounter(int i);
    Texture texture;

    void incrementCounter();

    void render(SDL_Renderer *pRenderer);
};


#endif //MARVEL_VS_CAPCOM_ROUNDCOUNTER_H

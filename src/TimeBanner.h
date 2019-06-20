//
// Created by IgVelasco on 6/20/19.
//

#ifndef MARVEL_VS_CAPCOM_TIMEBANNER_H
#define MARVEL_VS_CAPCOM_TIMEBANNER_H


#include "tools/ImageLoader/ImageLoader.h"

class TimeBanner {
public:
    TimeBanner(int posX);
    void load(SDL_Renderer *m_Renderer);
    void render(SDL_Renderer *mRenderer);


    int digit;
private:
    ImageLoader* loader;
    Texture texture;

    int position;
};


#endif //MARVEL_VS_CAPCOM_TIMEBANNER_H

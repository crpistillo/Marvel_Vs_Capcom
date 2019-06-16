/*
 * Music.h
 *
 *  Created on: 16 jun. 2019
 *      Author: caropistillo
 */

#ifndef MARVEL_VS_CAPCOM_MUSIC_H
#define MARVEL_VS_CAPCOM_MUSIC_H

#include <SDL2/SDL_mixer.h>
#include "tools/logger/Logger.h"
#include <mutex>

class Music
{

private:
    Mix_Music *gMusic;
    //Logger* logger;
    bool soundOn;

public:
    Music();
    bool initialize();
    void loadMusic();
    void free();
    void playBackGroundMusic(int soundKey);
    std::mutex m;



};

#endif


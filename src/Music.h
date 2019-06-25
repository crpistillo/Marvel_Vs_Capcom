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
#include "data_structs.h"
#include "Timer.h"
#include <mutex>

 class Music
{

 private:

     //Logger* logger;
    bool soundOn;
    bool effectsOn;
    Mix_Music *gMusic;
    Mix_Chunk *punch;
    Mix_Chunk *strongPunch;
    Mix_Chunk *kick;
    Mix_Chunk *strongKick;
    Mix_Chunk* hitMiss;
    Mix_Chunk* jump;
    Mix_Chunk* throws;
    Mix_Chunk* falling;
    Mix_Chunk* projectile;
    Mix_Chunk* spiderIntro;
    Mix_Chunk* wolverIntro;
    Mix_Chunk* ironIntro;
    Mix_Chunk* ryuIntro;

    Timer* timer;
    bool firstTime;

 public:

    Music();
    bool initialize();
    void endGame();
    void playHitMiss();
    void loadMusic();
    void playPunch();
    void playKick();
    void playStrongPunch();
    void playStrongKick();
    void playJump();
    void playFalling();
    void playThrow();
    void playProjectile();
    void playSpiderIntro();
    void playWolverIntro();
    void playIronIntro();
    void playRyuIntro();
    void loadEffects();
    void free();
    void playBackGroundMusic(int soundKey);
    void updateEffects(character_updater_t *updater);



 };

 #endif

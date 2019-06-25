/*
 * Music.cpp
 *
 *  Created on: 16 jun. 2019
 *      Author: caropistillo
 */

#include "Music.h"
#include "InputManager.h"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

Music::Music() {
    this->gMusic = NULL;
    this->punch = NULL;
    this->strongPunch = NULL;
    this->kick = NULL;
    this->strongKick = NULL;
    this->hitMiss = NULL;
    this->jump = NULL;
    this->throws = NULL;
    this->falling = NULL;
    this->spiderIntro = NULL;
    this->wolverIntro = NULL;
    this->soundOn = true;
    effectsOn = false;
    this->timer = new Timer(2);
}

bool Music::initialize() {
    //Inicializo SDL MIxer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        //logger->log("SDL_mixer no pudo inicializarse", ERROR);
        return false;
    }
    return true;
}


void Music::loadMusic() {
    this->gMusic = Mix_LoadMUS("music/music.wav");
    if (gMusic == NULL) {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        //logger->log("No se pudo cargar la musica de fondo", ERROR);
    }
}

void Music::loadEffects() {
    this->punch = Mix_LoadWAV("music/weakPunch.wav");
    if (punch == NULL)
        cout << "Failed to load punch effect! SDL_mixer Error: " << Mix_GetError() << endl;

    this->hitMiss = Mix_LoadWAV("music/hitMiss.wav");
    if (hitMiss == NULL)
        cout << "Failed to load hitMiss effect! SDL_mixer Error: " << Mix_GetError() << endl;

    this->kick = Mix_LoadWAV("music/weakKick.wav");
    if (kick == NULL)
        cout << "Failed to load kick effect! SDL_mixer Error: " << Mix_GetError() << endl;

    this->strongPunch = Mix_LoadWAV("music/strongPunch.wav");
    if (strongPunch == NULL)
        cout << "Failed to load strongPunch effect! SDL_mixer Error: " << Mix_GetError() << endl;

    this->strongKick = Mix_LoadWAV("music/strongKick.wav");
    if (strongKick == NULL)
        cout << "Failed to load strongKick effect! SDL_mixer Error: " << Mix_GetError() << endl;

    this->jump = Mix_LoadWAV("music/jump.wav");
    if (jump == NULL)
        cout << "Failed to load jump effect! SDL_mixer Error: " << Mix_GetError() << endl;

    this->falling = Mix_LoadWAV("music/fall.wav");
    if (falling == NULL)
        cout << "Failed to load falling effect! SDL_mixer Error: " << Mix_GetError() << endl;

    this->throws = Mix_LoadWAV("music/throw.wav");
    if (falling == NULL)
        cout << "Failed to load throw effect! SDL_mixer Error: " << Mix_GetError() << endl;

    this->spiderIntro = Mix_LoadWAV("music/spiderIntro.wav");
    if (falling == NULL)
        cout << "Failed to load spiderIntro effect! SDL_mixer Error: " << Mix_GetError() << endl;

    this->wolverIntro = Mix_LoadWAV("music/wolverIntro.wav");
    if (falling == NULL)
        cout << "Failed to load wolverIntro effect! SDL_mixer Error: " << Mix_GetError() << endl;

}

void Music::free() {
    Mix_FreeMusic(gMusic);
    Mix_FreeChunk(punch);
    Mix_FreeChunk(strongPunch);
    Mix_FreeChunk(hitMiss);
    Mix_FreeChunk(kick);
    Mix_FreeChunk(strongKick);
    Mix_FreeChunk(jump);
    Mix_FreeChunk(falling);
    Mix_FreeChunk(throws);
    Mix_FreeChunk(spiderIntro);
    Mix_FreeChunk(wolverIntro);
    gMusic = NULL;
    punch = NULL;
    kick = NULL;
    strongPunch = NULL;
    strongKick = NULL;
    hitMiss = NULL;
    jump = NULL;
    falling = NULL;
    throws = NULL;
    spiderIntro = NULL;
    wolverIntro = NULL;

}

void Music::playPunch() {
    Mix_PlayChannel(-1, punch, 0);
}

void Music::playHitMiss() {
    Mix_PlayChannel(-1, hitMiss, 0);
}

void Music::playKick() {
    Mix_PlayChannel(-1, kick, 0);
}

void Music::playStrongPunch() {
    Mix_PlayChannel(-1, strongPunch, 0);
}

void Music::playStrongKick() {
    Mix_PlayChannel(-1, strongKick, 0);
}

void Music::playJump() {
    Mix_PlayChannel(-1, jump, 0);
}

void Music::playFalling() {
    Mix_PlayChannel(-1, falling, 0);
}

void Music::playThrow() {
    Mix_PlayChannel(-1, throws, 0);
}

void Music::playSpiderIntro() {
    Mix_PlayChannel(-1, spiderIntro, 0);
}

void Music::playWolverIntro() {
    Mix_PlayChannel(-1, wolverIntro, 0);
}

void Music::updateEffects(character_updater_t *updater) {
    if (effectsOn) {
        if (updater->effect == WEAK_PUNCH) {
            playPunch();
        } else if (updater->effect == WEAK_KICK) {
            playKick();
        } else if (updater->effect == HIT_MISS) {
            playHitMiss();
        } else if (updater->effect == STRONG_PUNCH) {
            playStrongPunch();
        } else if (updater->effect == STRONG_KICK) {
            playStrongKick();
        } else if (updater->effect == JUMP) {
            playJump();
        } else if (updater->effect == FALL) {
            playFalling();
        } else if (updater->effect == THROWS) {
            playThrow();
        } else if (updater->effect == SPIDERINTRO) {
            playSpiderIntro();
        } else if (updater->effect == WOLVERINTRO) {
            playWolverIntro();
        }


    }


}


void Music::playBackGroundMusic(int soundKey) {
    InputManager *inputManager = InputManager::getInstance();
    bool input = false;
    if(timer->getTimeLeft() <= 0){
        input = inputManager->isKeyDown(soundKey);
    }

    if (input) {
        //m.lock();
        timer->resetTimer();
        cout << "Se detecta PLAYMUSIC" << endl;

        if (!soundOn) {
            if (Mix_PlayingMusic() == 0 && !soundOn) {
                //Play the music
                Mix_PlayMusic(gMusic, -1);
                effectsOn = true;
            }

                //If music is being played
            else {
                //If the music is paused
                if (Mix_PausedMusic() == 1 && soundOn) {
                    //Resume the music
                    Mix_ResumeMusic();
                }
                    //If the music is playing
                else {
                    //Pause the music
                    Mix_PauseMusic();
                    effectsOn = false;
                }
            }
            soundOn = true;
        } else {
            Mix_HaltMusic();
            soundOn = false;
            effectsOn = false;
        }

        //m.unlock();

        while (inputManager->isKeyUp(soundKey))
            continue;
    }
}




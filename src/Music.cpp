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

Music::Music()
{
	this->gMusic = NULL;
	this->soundOn = true;
}

bool Music::initialize()
{
	//Inicializo SDL MIxer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		//logger->log("SDL_mixer no pudo inicializarse", ERROR);
		return false;
	}
	return true;
}

void Music::loadMusic(const char *file) //pasar path a string
{
	this->gMusic = Mix_LoadMUS(file);
	if (gMusic == NULL)
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		//logger->log("No se pudo cargar la musica de fondo", ERROR);
	}
}

void Music::free()
{
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
}

void Music::playBackGroundMusic(int soundKey)
{
	InputManager *inputManager = InputManager::getInstance();
	if (inputManager->isKeyDown(soundKey))
	{
		m.lock();

		cout<<"Se detecta PLAYMUSIC"<<endl;

		if (!soundOn)
		{
			if (Mix_PlayingMusic() == 0 && !soundOn)
			{
				//Play the music
				Mix_PlayMusic(gMusic, -1);
			}

			//If music is being played
			else
			{
				//If the music is paused
				if (Mix_PausedMusic() == 1 && soundOn)
				{
					//Resume the music
					Mix_ResumeMusic();
				}
				//If the music is playing
				else
				{
					//Pause the music
					Mix_PauseMusic();
				}
			}
			soundOn = true;
		}

		else
		{
			Mix_HaltMusic();
			soundOn = false;
		}

		m.unlock();

		while(inputManager->isKeyUp(soundKey))
		continue;
	}
}



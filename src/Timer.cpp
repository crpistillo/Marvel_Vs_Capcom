//
// Created by IgVelasco on 6/19/19.
//

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#include <iostream>
#include "Timer.h"

Timer::Timer(int maxTime) {
    SDL_Init(SDL_INIT_TIMER);
    this->maxTime = maxTime;
    initialTime = SDL_GetTicks()/1000;
    currentTime = SDL_GetTicks()/1000;
}

int Timer::getFirstDigit() {
    currentTime = SDL_GetTicks() / 1000;
    int time = maxTime - (currentTime - initialTime);
    return time / 10;
}

int Timer::getSecondDigit() {
    currentTime = SDL_GetTicks() / 1000;
    int time = maxTime - (currentTime - initialTime);
    return  time % 10;
}

void Timer::resetTimer() {
    initialTime = SDL_GetTicks() / 1000;
    getTimeThatPass();
}

int Timer::getTimeThatPass() {
    currentTime = SDL_GetTicks() / 1000;
    return (currentTime - initialTime);
}

void Timer::setDigits(character_updater_t *updater) {
    updater->firstDigitOfTime = getFirstDigit();
    updater->secondDigitOfTime = getSecondDigit();
}

int Timer::getTimeLeft() {
    currentTime = SDL_GetTicks()/1000;
    return maxTime - (currentTime - initialTime);
}





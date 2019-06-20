//
// Created by IgVelasco on 6/20/19.
//

#include <SDL2/SDL_types.h>
#include "RoundBanner.h"
const string bannerFilePath = "images/letreros/";
const string FILE_EXTENSION = ".png";


RoundBanner::RoundBanner() {
    loader = new ImageLoader((Uint8) 255, (Uint8) 255, (Uint8) 255);
    currentAction = FIGHTING;
}

void RoundBanner::load(SDL_Renderer *m_Renderer) {
    switch (currentAction) {
        case FIGHTING:
            break;
        case ROUNDWINNER:
            break;
        case NUMBEROFROUND:
            this->loader->loadActionSprite(bannerFilePath, "round", numberOfRound, FILE_EXTENSION, m_Renderer, &texture);
            break;
        case FIGHT:
            this->loader->loadActionSprite(bannerFilePath, "fight", 0, FILE_EXTENSION, m_Renderer, &texture);
            break;

    }
    //case:
}

void RoundBanner::render(SDL_Renderer *mRenderer) {
    if(!active)
        return;
    texture.render(100 , 200, 50, 50,mRenderer);

}

void RoundBanner::updateRoundSprites(round_info_t round) {
    active = true;
    currentAction = round.roundInfo;
    currentWinner = round.winner;
    numberOfRound = round.numberOfRound;
}


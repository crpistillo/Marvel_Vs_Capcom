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
            if(numberOfRound != 1)
                this->loader->loadActionSprite(bannerFilePath, "team", currentWinner, "winsR" + FILE_EXTENSION, m_Renderer, &texture);
            else
                this->loader->loadActionSprite(bannerFilePath, "round", numberOfRound, FILE_EXTENSION, m_Renderer, &texture);
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
    texture.render(200 , 150, 400, 200,mRenderer);

}

void RoundBanner::updateRoundSprites(round_info_t round) {
    active = true;
    currentAction = round.roundInfo;
    currentWinner = round.winner;
    numberOfRound = round.numberOfRound;
}


//
// Created by IgVelasco on 6/20/19.
//

#include <SDL_types.h>
#include "RoundBanner.h"



RoundBanner::RoundBanner() {
    loader = new ImageLoader((Uint8) 255, (Uint8) 255, (Uint8) 255);
}

void RoundBanner::load() {
    //case:
}

void RoundBanner::render(SDL_Renderer *mRenderer) {
    if(!active)
        return;
    //m_Texture.render(posX - camX, posY - camY, 1000, 700,mRenderer);

}

void RoundBanner::updateRoundSprites(round_info_t round) {
    active = true;
    currentAction = round.roundInfo;
    currentWinner = round.winner;
    numberOfRound = round.numberOfRound;
    numberOfRound = round.numberOfRound;
}


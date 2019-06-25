//
// Created by donatojp on 25/06/19.
//

#include "RoundCounter.h"

RoundCounter::RoundCounter(int i) {
    posX = i;
    counter = 0;
}


void RoundCounter::incrementCounter() {
    counter += 1;
    cout << counter << endl;
}

void RoundCounter::render(SDL_Renderer *pRenderer) {
    texture.loadFromFile("images/round_counter/" + to_string(counter) + ".png", pRenderer);
    texture.render(posX , 28, 27, 27, pRenderer);
}

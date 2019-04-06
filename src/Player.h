//
// Created by amaherok on 4/6/19.
//

#ifndef MARVEL_VS_CAPCOM_PLAYER_H
#define MARVEL_VS_CAPCOM_PLAYER_H


#include "Characters/Character.h"

class Player {
public:
    Player(Character* first, Character* second);
    void update(SDL_Renderer* renderer, int distance, int posContrincante);
    void render(SDL_Renderer* mRenderer, int camX, int camY, int posContrincante);
    void free();
    void changeCharacter();


    void loads(SDL_Renderer *pRenderer);

    int getPosX();

private:
    Character* currentCharacter;
    Character* firstCharacter;
    Character* secondCharacter;

};


#endif //MARVEL_VS_CAPCOM_PLAYER_H

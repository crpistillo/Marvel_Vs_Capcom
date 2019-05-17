//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_CHARACTERSERVER_H
#define MARVEL_VS_CAPCOM_CHARACTERSERVER_H

#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "../InputTable.h"
#include "../InputManager.h"
#include "../tools/ImageLoader/ImageLoader.h"
#include "../Controls/Controls.h"
#include "../tools/logger/Logger.h"
#include "../Characters/orientation_constants.h"

typedef enum actions {
    STANDING = S, JUMPINGLEFT = JL, JUMPINGRIGHT = JR, JUMPINGVERTICAL = JV, MAKINGINTRO = MI, DUCK = D,
    MOVING = M, WALKBACK = WB
} actions_t;

using namespace std;

class CharacterServer {
public:
    virtual~CharacterServer();

    //Maximum axis velocity of the character
    static const int CHARACTER_VEL = 10;

    virtual void load(SDL_Renderer *renderer, int posContrincante) = 0;

    //Takes key presses and adjusts the character's velocity
//	void handleEvent(SDL_Event &e, SDL_Renderer* renderer, int distance);
    //Shows the character on the screen
    int getPosX();
    int getPosY();
    int getWidth();
    int getSobrante();
    int getCentro();
    void update(int distance, int posContrincante, actions_t actionRecieved);
    void positionUpdate(int *x);
    void startIntro();

    actions_t currentAction;

protected:
    CharacterServer(
            int mPosX,
            int mPosY,
            int width,
            int sobrante,
            bool isLookingLeft,
            int widthSprite,
            int heightSprite,
            int anchoPantalla
    );
    bool isLookingLeft;
    //The X and Y offsets of the character
    int mPosX, mPosY;
    //The velocity of the character
    int mVelX, mVelY;

    int width, sobrante;

    int widthSprite, heightSprite;

    int INITIAL_POS_Y;

    int ZIndex;

    int anchoPantalla;


    int currentStandingSprite;
    int currentWalkingSprite;
    int currentJumpingSprite;
    int currentJumpingRightSprite;
    int currentJumpingLeftSprite;
    int currentWalkbackSprite;
    int currentIntroSprite;


    unsigned int lastTime;

private:

    virtual void resetSpriteVariables() = 0;

    virtual void stand() = 0;

    virtual void renderDuckSprite() = 0;

    virtual void moveRight( int distance, int posContrincante) = 0;

    virtual void moveLeft( int distance, int posContrincante) = 0;

    virtual void jumpVertical() = 0;

    virtual void jumpRight() = 0;

    virtual void jumpLeft() = 0;

    virtual void updateStand() = 0;

    virtual void makeIntro() = 0;
};

#endif //MARVEL_VS_CAPCOM_CHARACTERSERVER_H

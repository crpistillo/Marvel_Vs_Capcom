//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_CHARACTERSERVER_H
#define MARVEL_VS_CAPCOM_CHARACTERSERVER_H

#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "../InputTable.h"
#include "../InputManager.h"
#include "../Controls/Controls.h"
#include "../tools/logger/Logger.h"
#include "../action_constants.h"
#include "../data_structs.h"
/*
typedef enum actions {
    STANDING = S, JUMPINGLEFT = JL, JUMPINGRIGHT = JR, JUMPINGVERTICAL = JV, MAKINGINTRO = MI, DUCK = D,
    MOVING = M, WALKBACK = WB
} actions_t;
*/
using namespace std;

class CharacterServer {
public:
    virtual~CharacterServer();

    //Maximum axis velocity of the character
    static const int CHARACTER_VEL = 10;

   // virtual void load(SDL_Renderer *renderer, int posContrincante) = 0;

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
    virtual void makeBuilderStruct(character_builder_t *builder, bool firstTeam) = 0;
    void makeUpdaterStruct(character_updater_t* updater);
    virtual int getSpriteNumber() = 0;
    actions_t getCurrentAction();
    bool isStanding();


    int clientNumber;
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
            int anchoPantalla,
            int numberOfClient
    );

    int currentClient; //indica el cliente al que pertence este personaje

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
    int currentPunchSprite;
    int currentKickSprite;
    int currentPunchDownSprite;
    int currentKickDownSprite;

    int lastStandingSprite;
    int lastWalkingSprite;
    int lastJumpingSprite;
    int lastJumpingLeftSprite;
    int lastJumpingRightSprite;
    int lastWalkbackSprite;
    int lastIntroSprite;
    int lastPunchSprite;
    int lastKickSprite;
    int lastPunchDownSprite;
    int lastKickDownSprite;

    unsigned int lastTime;

    virtual void walkingSpriteUpdate();

private:

    virtual void resetSpriteVariables() = 0;

    virtual void stand();

    virtual void renderDuckSprite();

    virtual void moveRight( int distance, int posContrincante) = 0;

    virtual void moveLeft( int distance, int posContrincante) = 0;

    virtual void jumpVertical();

    virtual void jump(int *currentSprite, int lastSprite);

    virtual void jumpRight();

    virtual void jumpLeft();

    virtual void updateStand();

    virtual void makeIntro();

    virtual void punch();

    virtual void kick();

    virtual void punchDown();

    virtual void kickDown();


};

#endif //MARVEL_VS_CAPCOM_CHARACTERSERVER_H

//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_CHARACTERSERVER_H
#define MARVEL_VS_CAPCOM_CHARACTERSERVER_H

#include <SDL2/SDL_events.h>
#include "../../Texture/Texture.h"
#include "../../InputTable.h"
#include "../../InputManager.h"
#include "../../Controls/Controls.h"
#include "../../tools/logger/Logger.h"
#include "../../action_constants.h"
#include "../../data_structs.h"
#include <vector>
#include "../../Box.h"
#include "../Projectiles/ProjectileServer.h"
#include<mutex>
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
    virtual void update(int distance, int posEnemy, actions_t actionRecieved, Box *boxEnemy);
    void positionUpdate(int *x);
    void startIntro();
    virtual void makeBuilderStruct(character_builder_t *builder, bool firstTeam) = 0;
    void makeUpdaterStruct(character_updater_t* updater);
    virtual int getSpriteNumber() = 0;
    actions_t getCurrentAction();
    bool isStanding();
    //vector<Box*> getColisionable();
    Box* getColisionable();
    void quitarVida(float danio);


    int clientNumber;
    actions_t currentAction;

    bool inTheGround();

    virtual bool isProjectileActive();

    virtual ProjectileServer *getProjectile();

    std::mutex m;

    virtual bool isProjectileHurting();

    bool isHurting();
    float getVida();

protected:
    CharacterServer(int mPosX, int mPosY, int width, int sobrante, bool isLookingLeft, int widthSprite,
                    int heightSprite, int anchoPantalla, int numberOfClient);

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

    Box* characterBox; //por ahora una sola caja
    //vector<Box*> objetoColisionable;

    float vida;

	int widthStanding;
	int heightStanding;
	int widthWalking;
	int heightWalking;
	int widthDuck;
	int heightDuck;
	int widthPunch;
	int heightPunch;
	int widthPunchDown;
	int heightPunchDown;
	int widthKick;
	int heightKick;
	int widthKickDown;
	int heightKickDown;
	int widthKickAir;
	int heightKickAir;
	int widthPunchAir;
	int heightPunchAir;
	int widthJumping;
	int heightJumping;
	int widthJumpingLeft;
	int heightJumpingLeft;


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
    int currentHurtingSprite;
    int currentThrowSprite;
    int currentHurtingAirSprite;
    int currentPunchAirSprite;
    int currentKickAirSprite;
    int currentThrowPowerSprite;
    int currentGripSprite;
    int currentFallingSprite;


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
    int lastHurtingSprite;
    int lastThrowSprite;
    int lastThrowPowerSprite;
    int lastGripSprite;
    int lastFallingSprite;
    int lastHurtingAirSprite;
    int lastPunchAirSprite;
    int lastKickAirSprite;

    virtual void throwPower();


    unsigned int lastTime;

    virtual void walkingSpriteUpdate();

    void resetSpriteVariables();

private:

    virtual void stand();

    virtual void renderDuckSprite();

    virtual void renderBlockSprite();

    virtual void moveRight(int distance, int vel, Box *boxOfEnemy, bool isGrounded) = 0;

    virtual void moveLeft(int distance, int vel, Box *boxOfEnemy, bool isGrounded) = 0;

    virtual void grip();

	virtual void throwCharacter();

	virtual void falling(int distance, int posContrincante, Box *boxOfEnemy);

    virtual void jumpVertical(Box *boxOfEnemy);

    virtual void jump(int *currentSprite, int lastSprite, Box *boxOfEnemy);

    virtual void jumpRight(int distance, Box *boxOfEnemy);

    virtual void jumpLeft(int distance, Box *boxOfEnemy);

    virtual void updateStand();

    virtual void makeIntro();

    virtual void punch(actions_t punch);

    virtual void kick(actions_t kick);

    virtual void punchDown(actions_t punchDown);

    virtual void kickDown(actions_t kickDown);


    void hurtingGround();

    void punchJumpVertical(actions_t punchVertical, Box *boxOfEnemy);

    void hurtingAir();

    void punchJumpLeft(actions_t punchJumpLeft, int distance, Box *boxOfEnemy);

    void punchJumpRight(actions_t punchJumpRight, int distance, Box *boxOfEnemy);

    void kickJumpVertical(actions_t kickVertical, Box *boxOfEnemy);

    void kickJumpRight(actions_t kickJumpRight, int distance, Box *boxOfEnemy);

    void kickJumpLeft(actions_t kickJumpLeft, int distance, Box *boxOfEnemy);

    void normalAction(int *currentSprite, int *lastSprite, actions_t finalAction);

    void airActions(int *currentSprite, int lastSprite, actions_t nextAction, int airSprite, int lastAirSprite);
};

#endif //MARVEL_VS_CAPCOM_CHARACTERSERVER_H

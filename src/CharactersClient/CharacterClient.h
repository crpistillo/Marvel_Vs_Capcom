//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_CHARACTERCLIENT_H
#define MARVEL_VS_CAPCOM_CHARACTERCLIENT_H



#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "../InputTable.h"
#include "../InputManager.h"
#include "../tools/ImageLoader/ImageLoader.h"
#include "../Controls/Controls.h"
#include "../tools/logger/Logger.h"
#include "../Characters/orientation_constants.h"


using namespace std;

class CharacterClient {
public:
    virtual~CharacterClient();

    //Maximum axis velocity of the character
    static const int CHARACTER_VEL = 10;

    virtual void load(SDL_Renderer *renderer, int posContrincante) = 0;

    //Takes key presses and adjusts the character's velocity
//	void handleEvent(SDL_Event &e, SDL_Renderer* renderer, int distance);
    //Shows the character on the screen

    void render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante);
    int getPosX();
    int getPosY();
    int getWidth();
    int getSobrante();
    int getCentro();
    actions_t getNewAction(SDL_Renderer *renderer, int distance, int posContrincante);
    void positionUpdate(int *x);
    void setControls(Controls *controls);
    void startIntro();
    Controls *getControls();
    bool isMoving();
    void setZIndex(int z);
    int getZIndex();
    void setFilepath(string fp);

    actions_t currentAction;
protected:
    CharacterClient(
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

    int width, sobrante;

    int widthSprite, heightSprite;

    int ZIndex;

    int anchoPantalla;

    string characterFilepath;

    Texture m_Texture;
    ImageLoader *loader;

    int currentStandingSprite;
    int currentWalkingSprite;
    int currentJumpingSprite;
    int currentJumpingRightSprite;
    int currentJumpingLeftSprite;
    int currentWalkbackSprite;
    int currentIntroSprite;

    Controls *characterControls;

    unsigned int lastTime;

  //  int mVelX, mVelY;


    /*
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

     */
};


#endif //MARVEL_VS_CAPCOM_CHARACTERCLIENT_H

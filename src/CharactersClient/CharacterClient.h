//
// Created by IgVelasco on 5/17/19.
//

#ifndef MARVEL_VS_CAPCOM_CHARACTERCLIENT_H
#define MARVEL_VS_CAPCOM_CHARACTERCLIENT_H



#include <SDL2/SDL_events.h>
#include "../Texture/Texture.h"
#include "../InputTable.h"
#include "../InputManager.h"
#include "../Controls/Controls.h"
#include "ProjectileClient.h"
#include "../tools/logger/Logger.h"
#include "../action_constants.h"
#include "../Renderizable.h"
#include "../data_structs.h"


using namespace std;

class CharacterClient: public Renderizable
{
public:
    virtual~CharacterClient();

    //Maximum axis velocity of the character
    static const int CHARACTER_VEL = 10;

    virtual void load(SDL_Renderer *renderer, int posContrincante) = 0;

    //Takes key presses and adjusts the character's velocity
//	void handleEvent(SDL_Event &e, SDL_Renderer* renderer, int distance);
    //Shows the character on the screen
    ProjectileClient* getProjectile();
    virtual void render(SDL_Renderer *mRenderer, int camX, int camY, int posContrincante);
    int getPosX();
    int getPosY();
    int getWidth();
    int getSobrante();
    int getCentro();

    void positionUpdate(int *x);
    void setControls(Controls *controls);
    void startIntro();
    Controls *getControls();
    bool isMoving();
    void setZIndex(int z);
    int getZIndex();
    void setFilepath(string fp);

    actions_t currentAction;

    int clientNumber;
    bool getDirection();

    void update(character_updater_t *ptr);

    virtual void loadBanner(SDL_Renderer *renderer) = 0;

    void renderBanner(SDL_Renderer *mRenderer, bool isCurrent);

    Texture characterLeftCurrentBanner;
    Texture characterRightCurrentBanner;
    Texture characterLeftSecondaryBanner;
    Texture characterRightSecondaryBanner;

    ProjectileClient* projectile;

    int vida;

protected:
    CharacterClient(int mPosX, int mPosY, int width, int sobrante, bool isLookingLeft, int widthSprite,
                    int heightSprite, int anchoPantalla, int numOfClient);
    bool isLookingLeft;
    //The X and Y offsets of the character
    int mPosX, mPosY;
    //The velocity of the character

    int width, sobrante;

    int widthSprite, heightSprite;

    int ZIndex;

    int anchoPantalla;

    string characterFilePath;

    Texture m_Texture;

    ImageLoader *loader;

    int currentSprite;

    Controls *characterControls;

    unsigned int lastTime;

  //  int mVelX, mVelY;


};


#endif //MARVEL_VS_CAPCOM_CHARACTERCLIENT_H

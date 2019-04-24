//
// Created by amaherok on 3/26/19.
//

#ifndef MARVEL_VS_CAPCOM_MCGAME_H
#define MARVEL_VS_CAPCOM_MCGAME_H

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>

#include "Characters/Spiderman.h"
#include "Characters/Wolverine.h"
#include "tools/FPSManager/FPSManager.h"
#include "Texture/Texture.h"
#include "tools/logger/Logger.h"
#include "InputManager.h"
#include "Player.h"
#include "Layer.h"
#include "Parallax.h"
#include "tools/json/ConfigFileParser/ConfigFileParser.h"

using namespace std;


class MCGame {
private:
    bool m_Running;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    SDL_Joystick* gGameController = NULL;
    Logger* logger;
    // Scene textures
    Texture frontGroundTexture;
    Texture middleGroundTexture;
    Texture backGroundTexture;
    Layer* middleGround;
    Layer* backGround;
    Layer* frontGround;
    Player* player1;
    Player* player2;
    Parallax* parallaxController;
    json config;

    void loadGroundTextureByZIndex();


protected:
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;


public:
    MCGame(json config, int ancho, int alto);
    ~MCGame(){}
    void init() { m_Running = true; }
    bool init(const char* title, int xpos, int ypos, int width, int
    height, int flags);
    void run();
    void render();
    void update();
    void handleEvents();
    void clean();
    SDL_Rect camera;
};


#endif //MARVEL_VS_CAPCOM_MCGAME_H

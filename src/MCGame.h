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

using namespace std;


class MCGame {
private:
    bool m_Running;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    SDL_Joystick* gGameController = NULL;
    Texture m_Texture; // the new SDL_Texture variable
    SDL_Rect* m_sourceRectangle; // the first rectangle
    SDL_Rect* m_destinationRectangle; // another rectangle
    Logger* logger;
    // Scene textures
    Texture g_BackgroundTexture;
    Spiderman* spiderman;
    Wolverine* wolverine;

public:
    MCGame(Logger* logger);
    ~MCGame(){}
    void init() { m_Running = true; }
    bool init(const char* title, int xpos, int ypos, int width, int
    height, int flags);
    void run();
    void render();
    void update();
    void handleEvents();
    void clean();
};


#endif //MARVEL_VS_CAPCOM_MCGAME_H

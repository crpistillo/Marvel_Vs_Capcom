//
// Created by amaherok on 3/26/19.
//

#ifndef MARVEL_VS_CAPCOM_MCGAME_H
#define MARVEL_VS_CAPCOM_MCGAME_H

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>

#include "CharactersClient/SpidermanClient.h"
#include "CharactersClient/WolverineClient.h"
#include "CharactersClient/IronManClient.h"
#include "CharactersClient/RyuClient.h"

#include "tools/FPSManager/FPSManager.h"
#include "Texture/Texture.h"
#include "tools/logger/Logger.h"
#include "InputManager.h"
#include "Player.h"
#include "Layer.h"
#include "Parallax.h"
#include "tools/json/ConfigFileParser/ConfigFileParser.h"
#include "Client/TCPClient.h"
#include "data_structs.h"
#include "Constants.h"
#include "ClientCursor.h"
#include "RoundBanner.h"
#include "TimeBanner.h"
#include <mutex>


#define FIRST_DIGIT_POSITION 370
#define SECOND_DIGIT_POSITION 400



using namespace std;


class MCGame {
private:
    bool m_Running;
    bool threadRunning;
    bool appCloseFromMenu;
    bool endgame;
    SDL_Window *m_Window;
    SDL_Renderer *m_Renderer;
    Logger *logger;
    // Scene textures
    Texture frontGroundTexture;
    Texture middleGroundTexture;
    Texture backGroundTexture;
    Texture menuTexture;
    Texture cliente1;
    Texture cliente2;
    Texture cliente3;
    Texture cliente4;
    Texture banner;
    Texture endgame_image;
    Layer *middleGround;
    Layer *backGround;
    Layer *frontGround;
    Player *players[2];
    Parallax *parallaxController;
    Controls *clientControls;
    json config;
    TCPClient *tcpClient;
    CharacterClient *characters[4];

    void loadGroundTextureByZIndex();

    Constants *constants;

    void action_update();

    std::mutex m;
    std::mutex threadRunning_mtx;

    void sendMenuEvents();

    void setThreadRunning(bool condition);
    bool getRunningThread();

    ClientCursor *clientCursors[4];

    void renderMenuBackImage();

    void sendMenuAlive(Uint32* timer);


protected:
    int SCREEN_WIDTH;


public:
    MCGame(json config, int ancho, int alto, TCPClient *client);

    ~MCGame() {}

    void init() { m_Running = true; }

    bool init(const char *title, int xpos, int ypos, int width, int
    height, int flags);

    void run();

    void menu();

    void render();

    void update();

    void handleEvents();

    void clean();

    SDL_Rect camera;

    CharacterClient *characterBuild(character_builder_t *builder, int clientNumber);


    void loadInitialTextures();

    void runMenu();

    void updateMenu();

    void renderMenu();

    void loadSelectedCharacters();


    void setCursors();

    int numberOfPlayers;

    bool isActive();

    bool isSending;
    int team;

    bool isRunning();

    static void static_signalHandlerClient(int sigNum);

    int maxTimeouts = 0;

    std::mutex pipe_mtx;

    void disableRoundSprites();

    RoundBanner *roundBanner;
    TimeBanner* timeBanner[2];
};


#endif //MARVEL_VS_CAPCOM_MCGAME_H

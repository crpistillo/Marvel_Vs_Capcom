//
// Created by amaherok on 3/26/19.
//

#include "MCGame.h"
#include "Controls/WASDControls.h"
#include "Controls/ArrowControls.h"
#include <queue>
#include <thread>
#include<iostream>
#include <sys/poll.h>
#include "signal_handler.h"
#include "RoundBanner.h"
#include "TimeBanner.h"
#include <csignal>




using namespace std;

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

const int SCREEN_FPS = 60;

int centerBefore, centerLater = -1000;
bool firstTime = true;

void orderBackgroundsByZIndex(json *backgroundList);



bool MCGame::init(const char *title, int xpos, int ypos, int width, int height, int flags) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

        logger->log("SDL no se pudo inicializar.", ERROR);
        return false;

    } else {
        logger->log("SDL Inicializó correctamente.", INFO);

        m_Window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (m_Window == 0) {

            logger->log("Fallo al crear ventana.", ERROR);
            return false;

        } else {
            logger->log("Ventana creada exitosamente.", INFO);

            m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (m_Renderer == 0) {
                logger->log("Fallo al crear Renderer", ERROR);

                return false;
            } else {
                logger->log("Renderer creado exitosamente.", INFO);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    logger->log("SDL_image no pudo inicializarse.", ERROR);
                    return false;
                }
            }
        }
    }


    roundBanner = new RoundBanner();
    timeBanner[0] = new TimeBanner(FIRST_DIGIT_POSITION);
    timeBanner[1] = new TimeBanner(SECOND_DIGIT_POSITION);


    Texture* waiting = new Texture();
    SDL_SetRenderDrawColor(m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_Renderer); // clear the renderer to the draw color

    waiting->loadFromFile("images/waiting.png", m_Renderer);
    waiting->render(0, 0, 800, 600, m_Renderer);
    SDL_RenderPresent(m_Renderer); // draw to the screen
    // everything inited successfully,
    return true;
}

void MCGame::loadInitialTextures() {
    players[0]->loads(m_Renderer, 3200);
    players[1]->loads(m_Renderer, 0);
    loadGroundTextureByZIndex();
    banner.loadFromFile("images/barras/banner.png",m_Renderer);
}

void MCGame::loadGroundTextureByZIndex() {
    json firstBackgroundConfig = this->config["battlefield"][0]["background"];
    json secondBackgroundConfig = this->config["battlefield"][1]["background"];
    json thirdBackgroundConfig = this->config["battlefield"][2]["background"];

    json backgroundsList[3] = {firstBackgroundConfig, secondBackgroundConfig, thirdBackgroundConfig};

    orderBackgroundsByZIndex(
            backgroundsList);  //Ordena la lista de backgrounds del config.json de menor a mayor segun z index.

    string backgroundFilepath = backgroundsList[0]["filepath"];
    string middlegroundFilepath = backgroundsList[1]["filepath"];
    string frontgroundFilepath = backgroundsList[2]["filepath"];

    backGroundTexture.loadFromFile(backgroundFilepath, m_Renderer);
    middleGroundTexture.loadFromFile(middlegroundFilepath, m_Renderer);
    frontGroundTexture.loadFromFile(frontgroundFilepath, m_Renderer);

    backGround->setZIndex(backgroundsList[0]["zindex"]);
    middleGround->setZIndex(backgroundsList[1]["zindex"]);
    frontGround->setZIndex(backgroundsList[2]["zindex"]);
}

void MCGame::static_signalHandlerClient(int sigNum) {
    TCPClient* tcp = TCPClient::getInstance();
    tcp->signalHandlerClient(sigNum);
}

MCGame::MCGame(json config, int ancho, int alto, TCPClient *client) {
    signal(SIGPIPE, signalHandler);
    signal(SIGPIPE, MCGame::static_signalHandlerClient);
    constants = (Constants *) (malloc(sizeof(Constants *)));
    this->logger = Logger::getInstance();
    this->SCREEN_WIDTH = ancho;
    this->tcpClient = client;
    m_Window = NULL;
    m_Renderer = NULL;
    m_Running = true;
    appCloseFromMenu = false;
    endgame = false;
    weHaveAWinner = false;
    winningTeam = -1;

    endgame_image.loadFromFile("images/gameOver.png",m_Renderer);

    ///////////////////////JSON///////////////////
    this->config = config;

    json spidermanConfig = config["characters"][0];
    json wolverineConfig = config["characters"][1];

    string msj;

    if (spidermanConfig["name"] != "spiderman") {
        string name = spidermanConfig["name"];
        string filepath = spidermanConfig["filepath"];
        msj = "No se reconoce al personaje '" + name + "'."
              + " Se intentara cargar las imagenes correspondiente al filepath: " + filepath
              + " como las imagenes del personaje 'spiderman'.";
        logger->log(msj, ERROR);
    }

    if (wolverineConfig["name"] != "wolverine") {
        string name = wolverineConfig["name"];
        string filepath = wolverineConfig["filepath"];
        msj = "No se reconoce al personaje '" + name + "'."
              + " Se cargaran las imagenes correspondiente al filepath: " + filepath
              + " como las imagenes del personaje 'wolverine'.";
        logger->log(msj, ERROR);
    }


    int widthSpiderman = spidermanConfig["width"];
    int heightSpiderman = spidermanConfig["height"];
    int widthWolverine = wolverineConfig["width"];
    int heightWolverine = wolverineConfig["height"];

    string spidermanPath = spidermanConfig["filepath"];
    if (spidermanPath != "images/spiderman/spiderman_")
        logger->log("Filepath para personaje Spiderman incorrecto. Error al cargar imagenes.", ERROR);
    string wolverinePath = wolverineConfig["filepath"];
    if (wolverinePath != "images/wolverine/wolverine_")
        logger->log("Filepath para personaje Wolverine incorrecto. Error al cargar imagenes.", ERROR);


    constants->widthSpiderman = widthSpiderman;
    constants->widthWolverine = widthWolverine;
    constants->heightSpiderman = heightSpiderman;
    constants->heightWolverine = heightWolverine;
    constants->zIndexSpiderman = spidermanConfig["zindex"];
    constants->zIndexWolverine = wolverineConfig["zindex"];


    constants->spidermanSobrante = widthSpiderman * 242 / 640;
    constants->spidermanAncho = widthSpiderman * 110 / 640;
    constants->wolverineSobrante = widthWolverine * 278 / 640;
    constants->wolverineAncho = widthWolverine * 87 / 640;

    constants->INITIAL_POS_X_PLAYER_ONE =
            ((LEVEL_WIDTH / 2) - constants->spidermanSobrante) - (constants->spidermanAncho / 2) - 200;
    constants->INITIAL_POS_X_PLAYER_TWO =
            ((LEVEL_WIDTH / 2) - constants->wolverineSobrante) - (constants->wolverineAncho / 2) + 200;

    logger->log("Creacion de personajes.", DEBUG);

    this->clientControls = new ArrowControls();

    //////////////////////////////////////////////////////////////////////

    this->numberOfPlayers = tcpClient->numberOfPlayers;//tcpClient->numberOfPlayers;

    if (numberOfPlayers == 2) {
        if (tcpClient->nclient == 0)
            team = 0;
        else
            team = 1;
    } else {
        if (tcpClient->nclient < 2)
            team = 0;
        else
            team = 1;
    }


    middleGround = new Layer(2400, 600, 3.33, 400); //3.33
    backGround = new Layer(1600, 600, 6.66667, 800); //6.715
    frontGround = new Layer(3200, 600, 0, 0);

    logger->log("Creacion de Parallax.", DEBUG);

    parallaxController = new Parallax(&middleGround, &backGround, &camera, &centerBefore, &centerLater, logger,
                                      SCREEN_WIDTH);
    //barra = new Barra(88,74,0.1,true);
}


void MCGame::action_update() {
    FPSManager fpsManager(25);

    while (true) {
        fpsManager.start();

        handleEvents();

        actions_t actionToSend = clientControls->getNewAction();
        if(!isActive() && actionToSend != DISCONNECTEDCLIENT){
            actions_t actionToSend = STANDING;
            tcpClient->socketClient->sendData(&actionToSend, sizeof(actionToSend));
        }else{
            pipe_mtx.lock();
            if(!tcpClient->isPipeBroken) {
                tcpClient->socketClient->sendData(&actionToSend, sizeof(actionToSend));
            }
            pipe_mtx.unlock();
        }
		if (!getRunningThread())
            break;

        fpsManager.stop();

    }
    std::unique_lock<std::mutex> lock(m);
    m_Running = false;
}

void MCGame::run() {
    m_Running = true;
    FPSManager fpsManager(SCREEN_FPS);
    logger->log("Inicio de Bucle MCGame-run.", DEBUG);

    bool endgame = false;

    std::thread send(&MCGame::action_update, this);
  //  std::thread alive(&MCGame::alive_action, this);
    maxTimeouts = 0;

    setThreadRunning(true);
    while (m_Running)
    {
        fpsManager.start();

        update();
        render();

        fpsManager.stop();
    }
    send.join();
    send.~thread();


    logger->log("Fin de Bucle MCGame-run.", DEBUG);
}

void orderRenderizableListByZIndex(Renderizable **list);

void MCGame::render() {
    logger->log("Inicio render.", DEBUG);
    SDL_SetRenderDrawColor(m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_Renderer); // clear the renderer to the draw color

    if(endgame){
        endgame_image.loadFromFile("images/gameOver.png",m_Renderer);
        endgame_image.render(0,0,800,600,m_Renderer);
    }
    else if(weHaveAWinner){
        winningTeam_background_image.loadFromFile("images/pantalla_final/fondo.png", m_Renderer);
        winningTeam_background_image.render(0,0,800,600,m_Renderer);
    }
    else {

        Renderizable *renderizables[5] = {&(*middleGround), &(*backGround), &(*frontGround), &(*players[0]),
                                          &(*players[1])};
        orderRenderizableListByZIndex(renderizables);


        //roundBanner->render(nullptr);

        for (int i = 0; i < 5; i++) {
            if (renderizables[i] == backGround) {
                backGround->render(camera.x, camera.y, m_Renderer, &backGroundTexture, nullptr);
            } else if (renderizables[i] == middleGround) {
                middleGround->render(camera.x, camera.y, m_Renderer, &middleGroundTexture, nullptr);
            } else if (renderizables[i] == frontGround) {
                frontGround->render(0, 0, m_Renderer, &frontGroundTexture, &camera);
                banner.render(0, 11, 800, 104, m_Renderer);

            } else if (renderizables[i] == players[1]) {
                players[1]->render(m_Renderer, camera.x, camera.y, players[0]->getCentro());
            } else if (renderizables[i] == players[0]) {
                players[0]->render(m_Renderer, camera.x, camera.y, players[1]->getCentro());
            }
        }
        players[0]->renderProyectiles(m_Renderer, camera.x, camera.y);
        players[1]->renderProyectiles(m_Renderer, camera.x, camera.y);

        roundBanner->render(m_Renderer);
        timeBanner[0]->render(m_Renderer);
        timeBanner[1]->render(m_Renderer);
        //barra->render(m_Renderer);
        players[0]->renderBanner(m_Renderer);
        players[1]->renderBanner(m_Renderer);
    }
    logger->log("Fin render.", DEBUG);



    SDL_RenderPresent(m_Renderer); // draw to the screen
}

void orderRenderizableListByZIndex(Renderizable **list) {

    int pos_sel = 0;
    Renderizable *aux;

    for (int i = 4; i >= 0; i--) {

        for (int x = 0; x <= i; x++) {

            if (list[x]->getZIndex() > list[pos_sel]->getZIndex())
                pos_sel = x;
        }
        aux = list[i];
        list[i] = list[pos_sel];
        list[pos_sel] = aux;
        pos_sel = 0;
    }
}

void MCGame::clean() {
    //m_Texture.free();
    free(constants);
    logger->log("Inicio limpieza MCGame.", INFO);
    delete players[0];
    delete players[1];
    logger->log("Borrado de jugadores finalizado.", DEBUG);
    frontGroundTexture.free();
    middleGroundTexture.free();
    backGroundTexture.free();
    menuTexture.free();
    banner.free();
    cliente1.free();
    cliente2.free();
    cliente3.free();
    cliente4.free();
    logger->log("Liberacion de variables de fondo finalizado.", DEBUG);
    delete backGround;
    delete middleGround;
    delete parallaxController;
    logger->log("Borrado de fondos finalizado.", DEBUG);
    SDL_DestroyWindow(m_Window);
    SDL_DestroyRenderer(m_Renderer);
    SDL_Quit();
    logger->log("Fin clean MCGame", DEBUG);
}

void MCGame::handleEvents() {
    int windowsClosed = 0;
    InputManager *inputManager = InputManager::getInstance();
    inputManager->update();

    if (inputManager->closeWindowRequested()) {
        SDL_HideWindow(m_Window);
        setThreadRunning(false);
    }

}

void MCGame::update() {

	struct pollfd fds[1];
	memset(fds, 0, sizeof(fds));

	fds[0].fd = tcpClient->socketClient->get_fd();
	fds[0].events = POLLIN;

	int timeout = (1 * 50);

    int rc = poll(fds, 1, timeout);


    if (rc < 0)
    	cout << "Error en poll" << endl;

    else if (rc == 0){
        if(maxTimeouts > 200){
            cout<<"El server se ha desconectado. Fin del juego. "<<endl;
            tcpClient->socketClient->closeFd();
            tcpClient->socketClient->closeConnection();

            exit(1);
        }
        pipe_mtx.lock();
        tcpClient->isPipeBroken = true;
        pipe_mtx.unlock();
        maxTimeouts++;
    }else{
        pipe_mtx.lock();
        tcpClient->isPipeBroken = false;
        pipe_mtx.unlock();
		char buf1[sizeof(character_updater_t)];

		bool receiveCorrect = tcpClient->socketClient->reciveData(buf1, sizeof(character_updater_t));
        character_updater_t *updater = (character_updater_t *) buf1;

		if(updater->gameFinishedByDisconnections || !receiveCorrect ){
		    endgame = true;
		    return;
		}
		else if(updater->gameFinishedByWinningTeam){
		    weHaveAWinner = true;
		    winningTeam = updater->winningTeam;
            return;
		}


		if(updater->round.roundInfo != FIGHTING){
		    if(resetLifeBanners) {
                players[0]->resetLifeBanners();
                players[1]->resetLifeBanners();
                resetLifeBanners = false;
            }
            roundBanner->updateRoundSprites(updater->round);
            roundBanner->load(m_Renderer);
		} else
		    disableRoundSprites();

		timeBanner[0]->digit = updater->firstDigitOfTime;
		timeBanner[0]->load(m_Renderer);
		timeBanner[1]->digit = updater->secondDigitOfTime;
        timeBanner[1]->load(m_Renderer);


        if (updater->team == 0) {
            players[0]->update(updater, &isSending, 0 == team, tcpClient->nclient);
			players[0]->load(m_Renderer, players[1]->getCentro());
		} else {
            players[1]->update(updater, &isSending, 1 == team, tcpClient->nclient);
			players[1]->load(m_Renderer, players[0]->getCentro());
		}


        parallaxController->centerLayers(&players[0], &players[1]);
        maxTimeouts = 0 ;
        logger->log("Actualizacion parallax - MCGame.", DEBUG);
		parallaxController->doParallax(&players[0], &players[1], logger);
    }

}

CharacterClient *MCGame::characterBuild(character_builder_t *builder, int clientNumber) {
    CharacterClient *characterClient = nullptr;

    int pos = builder->pos;

    switch (builder->personaje) {
        case SPIDERMAN:
            characterClient = new SpidermanClient(pos,
                                                  !builder->isFirstTeam,
                                                  constants->widthSpiderman,
                                                  constants->heightSpiderman,
                                                  constants->spidermanSobrante,
                                                  constants->spidermanAncho,
                                                  constants->screenWidth, clientNumber);
            characterClient->setZIndex(constants->zIndexSpiderman);
            break;

        case WOLVERINE:
            characterClient = new WolverineClient(pos,
                                                  !builder->isFirstTeam,
                                                  constants->widthWolverine,
                                                  constants->heightWolverine,
                                                  constants->wolverineSobrante,
                                                  constants->wolverineAncho,
                                                  constants->screenWidth, clientNumber);
            characterClient->setZIndex(constants->zIndexWolverine);
            break;

        case IRONMAN:
            characterClient = new IronmanClient(pos,
                                                  !builder->isFirstTeam,
                                                  constants->widthSpiderman - 100,
                                                  constants->heightSpiderman,
                                                  constants->spidermanSobrante,
                                                  constants->spidermanAncho,
                                                  constants->screenWidth, clientNumber);
            characterClient->setZIndex(constants->zIndexSpiderman);
            break;

        case RYU:
            characterClient = new RyuClient(pos,
                                                  !builder->isFirstTeam,
                                                  constants->widthSpiderman,
                                                  constants->heightSpiderman,
                                                  constants->spidermanSobrante,
                                                  constants->spidermanAncho,
                                                  constants->screenWidth, clientNumber);
            characterClient->setZIndex(constants->zIndexSpiderman);
            break;
    }
    return characterClient;
}


void orderBackgroundsByZIndex(json *backgroundList) {

    int pos_sel = 0;
    json aux;

    for (int i = 2; i >= 0; i--) {

        for (int x = 0; x <= i; x++) {
            if (backgroundList[x]["zindex"] > backgroundList[pos_sel]["zindex"]) {
                pos_sel = x;
            }
        }

        aux = backgroundList[i];
        backgroundList[i] = backgroundList[pos_sel];
        backgroundList[pos_sel] = aux;
        pos_sel = 0;
    }
}


void MCGame::sendMenuEvents() {
    int charactersSelected = 0;
    FPSManager fpsManager(10);
    setThreadRunning(true);
    Uint32 timer = SDL_GetTicks();

    bool sendOnlyAlive = false;

    while (1) {
        fpsManager.start();

        handleEvents();
        menu_action_t menuActionToSend = clientControls->getNewMenuAction();
        if(menuActionToSend == MENU_WINDOWCLOSED){
        	appCloseFromMenu = true;
        	break;
        }

        /*if (menuActionToSend == ENTER && numberOfPlayers == 2 && charactersSelected == 0 && !sendOnlyAlive){
            menuActionToSend = SELECT;
            charactersSelected++;
        }*/
        if (menuActionToSend != INVALID_MENU_ACTION && !sendOnlyAlive) {
            tcpClient->socketClient->sendData(&menuActionToSend, sizeof(menuActionToSend));
        }

        sendMenuAlive(&timer);


        if (!getRunningThread())
            break;
        /*if (menuActionToSend == ENTER)
            sendOnlyAlive = true;*/
        fpsManager.stop();

    }
    std::unique_lock<std::mutex> lock(m);
    m_Running = false;

}

void MCGame::sendMenuAlive(Uint32* timer){
	menu_action_t alive = ALIVE_MENU;
	Uint32 actual_time = SDL_GetTicks();

	if((actual_time - *timer) > 1000 ){
		tcpClient->socketClient->sendData(&alive, sizeof(menu_action_t));
		*timer = actual_time;
	}

}


void MCGame::runMenu() {

    //Crear hilo que manda eventos de SDL
    std::thread sendMenuEventsThread(&MCGame::sendMenuEvents, this);
    setCursors();
    //Continuar con la ejecucion de MCGame::menu
    menu();
    sendMenuEventsThread.join();
    sendMenuEventsThread.~thread();
    if(appCloseFromMenu){
        tcpClient->socketClient->closeConnection();
        tcpClient->socketClient->closeFd();
    	exit(1);

    }
}

void MCGame::menu() {
    m.lock();
    m_Running = true;
    m.unlock();
    FPSManager fpsManager(SCREEN_FPS);
    logger->log("Inicio de Bucle MCGame-Menu.", DEBUG);


    while (isRunning()) {
        fpsManager.start();

        updateMenu();
        renderMenu();

        fpsManager.stop();
    }
    setThreadRunning(false);

    logger->log("Fin de Bucle MCGame-Menu.", DEBUG);

}

void MCGame::setThreadRunning(bool condition){
	std::unique_lock<std::mutex> lock(threadRunning_mtx);
	threadRunning = condition;
}

bool MCGame::getRunningThread(){
	std::unique_lock<std::mutex> lock(threadRunning_mtx);
	return threadRunning;
}


void MCGame::updateMenu() {

    cursor_updater_t *updaterMenu;
    char buf[sizeof(cursor_updater_t)];

    tcpClient->socketClient->reciveData(buf, sizeof(cursor_updater_t));
    updaterMenu = (cursor_updater_t *) buf;

    clientCursors[updaterMenu->cliente]->update(updaterMenu);

    if (updaterMenu->menuTerminated){
        std::unique_lock<std::mutex> lock(m);
        m_Running = false;
    }
}

void MCGame::renderMenu() {
    logger->log("Inicio render Menu.", DEBUG);
    SDL_SetRenderDrawColor(m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_Renderer);

    renderMenuBackImage();

    for (int i = 0; i < 4; i++)
        clientCursors[i]->render(this->m_Renderer);

    logger->log("Fin render Menu.", DEBUG);
    SDL_RenderPresent(m_Renderer); // draw to the screen

}

void MCGame::renderMenuBackImage() {
    Texture menuBackImage;
    menuBackImage.loadFromFile("images/menu4/Menu4.png", this->m_Renderer);
    menuBackImage.render(0, 0, 800, 600, m_Renderer);
}

void MCGame::loadSelectedCharacters() {

    int i = 0;
    for (auto &character : characters) {
        char buf1[sizeof(character_builder_t)];
        character_builder_t *builder;
        tcpClient->socketClient->reciveData(buf1, sizeof(character_builder_t));
        builder = (character_builder_t *) buf1;
        character = characterBuild(builder, i);
        i++;
    }

    players[0] = new Player(characters[0], characters[1]);
    players[1] = new Player(characters[2], characters[3]);

    int currentCharacter0;
    int currentCharacter1;
    tcpClient->socketClient->reciveData(&currentCharacter0, sizeof(int));
    tcpClient->socketClient->reciveData(&currentCharacter1, sizeof(int));

    players[0]->setCurrentCharacter(currentCharacter0, m_Renderer);
    players[1]->setCurrentCharacter(currentCharacter1, m_Renderer);

    players[0]->setBarra(true);
    players[1]->setBarra(false);

    isSending = (this->tcpClient->nclient) == players[team]->getCurrentCharacter()->clientNumber;


}

void MCGame::setCursors() {
    int posX, posY;
    bool visible;
    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            posX = 17;
            posY = 105;
        }
        if (i == 2) {
            posX = 17;
            posY = 403;
        }
        if (i == 3) {
            posX = 602;
            posY = 403;
        }
        if (i == 1) {
            posX = 602;
            posY = 105;
        }
        visible = !(numberOfPlayers == 2 && (i == 1 || i == 3));

        clientCursors[i] = new ClientCursor(posX, posY, this->m_Renderer, visible);
    }
    renderMenuBackImage();

    for (auto & clientCursor : clientCursors)
        clientCursor->render(this->m_Renderer);

    SDL_RenderPresent(m_Renderer);
}

bool MCGame::isActive() {
    std::unique_lock<std::mutex> lock(m);
        return isSending || numberOfPlayers == 2;
}

bool MCGame::isRunning() {
    std::unique_lock<std::mutex> lock(m);
    return m_Running;
}

void MCGame::disableRoundSprites() {
    resetLifeBanners = true;
    roundBanner->active = false;
}



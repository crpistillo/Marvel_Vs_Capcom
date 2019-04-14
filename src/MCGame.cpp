//
// Created by amaherok on 3/26/19.
//

#include "MCGame.h"
#include "Controls/WASDControls.h"
#include "Controls/ArrowControls.h"


using namespace std;

/*200 es el corrimiento a la izquierda desde el centro*/
//((LEVEL_WIDTH/2)-Spiderman::SOBRANTE)-(Spiderman::CHARACTER_WIDTH/2)-200;
const int INITIAL_POS_X_PLAYER_ONE = 864;
const int INITIAL_POS_X_PLAYER_TWO = 1222;


const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

const int SCREEN_FPS = 30;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int distancia;
int distancia2;
int centerBefore,centerLater=-1000;


SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

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

                player1->loads(m_Renderer);
                player2->loads(m_Renderer);

                backGroundTexture.loadFromFile("images/fondo.png", m_Renderer);
                middleGroundTexture.loadFromFile("images/montanas.png", m_Renderer);
                frontGroundTexture.loadFromFile("images/camino.png", m_Renderer);
            }
        }
    }

    // everything inited successfully,
    return true;
}

MCGame::MCGame(Logger* logger, json config){
	this->logger = logger;
	m_Window = NULL;
	m_Renderer = NULL;
	m_Running = false;
	this->config = config;


	Character* character1 = new Spiderman(INITIAL_POS_X_PLAYER_ONE, false);
    Character* character2 = new Wolverine(INITIAL_POS_X_PLAYER_ONE, false);

    Character* character3 = new Wolverine(INITIAL_POS_X_PLAYER_TWO, true);
    Character* character4 = new Spiderman(INITIAL_POS_X_PLAYER_TWO, true);

    logger->log("Creacion de controles.", DEBUG);

    Controls* controlPlayer2 = new WASDControls();
    Controls* controlPlayer1 = new ArrowControls();

    logger->log("Creacion de Jugadores.", DEBUG);

    player1 = new Player(character1, character2, controlPlayer1);
    player2 = new Player(character3, character4, controlPlayer2);

    middleGround = new Layer(2400, 600, 3.33, 400);//3.33
    backGround = new Layer(1600,600,6.66667,800);//6.715

    parallaxController = new Parallax(&middleGround, &backGround, &camera, &centerBefore, &centerLater);
}

void MCGame::run() {
	m_Running = true;
	FPSManager fpsManager(SCREEN_FPS);

	logger->log("Inicio de Bucle MCGame-run.", DEBUG);

	while(m_Running) {
		fpsManager.start();

		handleEvents();
		update();
		render();

		fpsManager.stop();
	}
	logger->log("Fin de Bucle MCGame-run.", DEBUG);
}

void MCGame::render() {
	SDL_SetRenderDrawColor( m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(m_Renderer); // clear the renderer to the draw color
	backGround->render(camera.x, camera.y, m_Renderer, &backGroundTexture);
    middleGround->render(camera.x, camera.y, m_Renderer, &middleGroundTexture);
    frontGroundTexture.render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, m_Renderer, &camera);

    player2->render(m_Renderer, camera.x, camera.y, player1->getPosX());
    player1->render(m_Renderer, camera.x, camera.y, player2->getPosX());
    SDL_RenderPresent(m_Renderer); // draw to the screen
}

void MCGame::clean() {
    //m_Texture.free();
    delete player1;
    delete player2;
    frontGroundTexture.free();
    middleGroundTexture.free();
    backGroundTexture.free();
    delete backGround;
    delete middleGround;
    delete parallaxController;
    SDL_DestroyWindow(m_Window);
    SDL_DestroyRenderer(m_Renderer);
    SDL_Quit();
    logger->log("Fin clean MCGame", DEBUG);
}

void MCGame::handleEvents() {
	InputManager* inputManager = InputManager::getInstance();
    inputManager->update();
    if(inputManager->quitRequested()) m_Running = false;
}

void MCGame::update() {

	//distancia = player1->getPosX() + (198/2) - player2->getPosX() + (157/2);
	//distancia2 = player2->getPosX() + (157/2) - player1->getPosX() + (198/2);
	if (player1->getCentro() > player2->getCentro()) {
		distancia = player1->getPosX()+player1->getSobrante()+player1->getWidth() - (player2->getPosX()+player2->getSobrante());
		distancia2 = player2->getPosX()+player2->getSobrante() - (player1->getPosX()+player1->getSobrante()+player1->getWidth());
	} else {
		distancia = player1->getPosX()+player1->getSobrante() - (player2->getPosX()+player2->getSobrante()+player2->getWidth());
		distancia2 = player2->getPosX()+player2->getSobrante()+player2->getWidth() - (player1->getPosX()+player1->getSobrante());
	}
    player1->update(m_Renderer, distancia, player2->getPosX());
    player2->update(m_Renderer, distancia2, player1->getPosX());

    parallaxController->doParallax(&player1,&player2);
}

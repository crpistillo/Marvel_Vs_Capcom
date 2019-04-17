//
// Created by amaherok on 3/26/19.
//

#include "MCGame.h"
#include "Controls/WASDControls.h"
#include "Controls/ArrowControls.h"
#include <queue>


using namespace std;




const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

const int SCREEN_FPS = 30;

int distancia;
int distancia2;
int centerBefore,centerLater=-1000;


void orderBackgroundsByZIndex(json* backgroundList);


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


                loadGroundTextureByZIndex();
            }
        }
    }

    // everything inited successfully,
    return true;
}

void MCGame::loadGroundTextureByZIndex(){
	json firstBackgroundConfig =  this->config["battlefield"][0]["background"];
	json secondBackgroundConfig =  this->config["battlefield"][1]["background"];
	json thirdBackgroundConfig =  this->config["battlefield"][2]["background"];

	json backgroundsList[3] = {firstBackgroundConfig, secondBackgroundConfig, thirdBackgroundConfig};

	orderBackgroundsByZIndex(backgroundsList);  //Ordena la lista de backgrounds del config.json de menor a mayor segun z index.

	string backgroundFilepath = backgroundsList[0]["filepath"];
	string middlegroundFilepath = backgroundsList[1]["filepath"];
	string frontgroundFilepath = backgroundsList[2]["filepath"];

	backGroundTexture.loadFromFile("images/" + backgroundFilepath, m_Renderer);
	middleGroundTexture.loadFromFile("images/" + middlegroundFilepath, m_Renderer);
	frontGroundTexture.loadFromFile("images/" + frontgroundFilepath, m_Renderer);

	backGround->setZIndex(backgroundsList[0]["zindex"]);
	middleGround->setZIndex(backgroundsList[1]["zindex"]);
	frontGround->setZIndex(backgroundsList[2]["zindex"]);
}



MCGame::MCGame(Logger* logger, json config, int ancho, int alto){
	this->logger = logger;
	this->SCREEN_WIDTH = ancho;
	this->SCREEN_HEIGHT = alto;
	m_Window = NULL;
	m_Renderer = NULL;
	m_Running = false;
	this->config = config;

	json spidermanConfig;
	json wolverineConfig;

	if(config["characters"][0]["name"] == "spiderman"){
		spidermanConfig = config["characters"][0];
		wolverineConfig = config["characters"][1];
	}
	else{
		spidermanConfig = config["characters"][1];
		wolverineConfig = config["characters"][0];
	}

	int widthSpiderman = spidermanConfig["width"];
	int heightSpiderman = spidermanConfig["height"];
	int widthWolverine = wolverineConfig["width"];
	int heightWolverine = wolverineConfig["height"];

	string spidermanPath = spidermanConfig["filepath"];
	if(spidermanPath != "images/spiderman/spiderman_")
		logger->log("Filepath para personaje Spiderman incorrecto. Error al cargar imagenes.", ERROR);
	string wolverinePath = wolverineConfig["filepath"];
	if(wolverinePath != "images/wolverine/wolverine_")
		logger->log("Filepath para personaje Wolverine incorrecto. Error al cargar imagenes.", ERROR);


	int spidermanSobrante = widthSpiderman*242/640;
	int spidermanAncho= widthSpiderman*110/640;
	int wolverineSobrante = widthWolverine*278/640;
	int wolverineAncho= widthWolverine*87/640;

	int INITIAL_POS_X_PLAYER_ONE = ((LEVEL_WIDTH/2)-spidermanSobrante)-(spidermanAncho/2)-200;
	int INITIAL_POS_X_PLAYER_TWO = ((LEVEL_WIDTH/2)-wolverineSobrante)-(wolverineAncho/2)+200;

	logger->log("Creacion de personajes.", DEBUG);

	Character* character1 = new Spiderman(INITIAL_POS_X_PLAYER_ONE, false, widthSpiderman, heightSpiderman, spidermanSobrante, spidermanAncho, SCREEN_WIDTH);
	character1->setZIndex(spidermanConfig["zindex"]);
	character1->setFilepath(spidermanPath);
    Character* character2 = new Wolverine(INITIAL_POS_X_PLAYER_ONE, false, widthWolverine, heightWolverine, wolverineSobrante, wolverineAncho, SCREEN_WIDTH);
    character2->setZIndex(wolverineConfig["zindex"]);
    character2->setFilepath(wolverinePath);


    Character* character3 = new Wolverine(INITIAL_POS_X_PLAYER_TWO, true, widthWolverine, heightWolverine, wolverineSobrante, wolverineAncho, SCREEN_WIDTH);
    character3->setZIndex(wolverineConfig["zindex"]);
    character3->setFilepath(wolverinePath);
    Character* character4 = new Spiderman(INITIAL_POS_X_PLAYER_TWO, true, widthSpiderman, heightSpiderman, spidermanSobrante, spidermanAncho, SCREEN_WIDTH);
    character4->setZIndex(spidermanConfig["zindex"]);
    character4->setFilepath(spidermanPath);

    logger->log("Creacion de controles.", DEBUG);

    Controls* controlPlayer2 = new WASDControls();
    Controls* controlPlayer1 = new ArrowControls();

    logger->log("Creacion de Jugadores.", DEBUG);

    player1 = new Player(character1, character2, controlPlayer1,logger);
    player2 = new Player(character3, character4, controlPlayer2,logger);

    logger->log("Definicion de Fondo.", DEBUG);	

    middleGround = new Layer(2400, 600, 3.33, 400);//3.33
    backGround = new Layer(1600,600,6.66667,800);//6.715
    frontGround = new Layer(3200,600,0,0);

    logger->log("Creacion de Parallax.", DEBUG);

    parallaxController = new Parallax(&middleGround, &backGround, &camera, &centerBefore, &centerLater, logger, SCREEN_WIDTH);
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

void orderRenderizableListByZIndex(Renderizable** list);

void MCGame::render() {
	logger->log("Inicio render.", DEBUG);	
	SDL_SetRenderDrawColor( m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(m_Renderer); // clear the renderer to the draw color

	Renderizable* renderizables[5] = {  &(*middleGround), &(*backGround), &(*frontGround) , &(*player1) , &(*player2)};
	orderRenderizableListByZIndex(renderizables);

	for(int i = 0; i < 5; i++){
		if(renderizables[i] == backGround){
			backGround->render(camera.x, camera.y, m_Renderer, &backGroundTexture, nullptr);
		}
		else if(renderizables[i] == middleGround){
			middleGround->render(camera.x, camera.y, m_Renderer, &middleGroundTexture, nullptr);
		}
		else if(renderizables[i] == frontGround){
			frontGround->render(0, 0, m_Renderer, &frontGroundTexture,&camera);
		}
		else if(renderizables[i] == player2){
			player2->render(m_Renderer, camera.x, camera.y, player1->getCentro(),logger);
		}
		else if(renderizables[i] == player1){
			player1->render(m_Renderer, camera.x, camera.y, player2->getCentro(),logger);
		}
	}
	logger->log("Fin render.", DEBUG);
    SDL_RenderPresent(m_Renderer); // draw to the screen
}

void orderRenderizableListByZIndex(Renderizable** list){

	int pos_sel = 0;
	Renderizable* aux;

	for(int i = 4; i >= 0; i--){

		for(int x=0; x<= i; x++){

			if(list[x]->getZIndex() > list[pos_sel]->getZIndex())
				pos_sel = x;

			aux = list[i];
			list[i] = list[pos_sel];
			list[pos_sel] = aux;
			pos_sel = 0;

		}
	}
}



void MCGame::clean() {
    //m_Texture.free();
    logger->log("Inicio limpieza MCGame.", INFO);
    delete player1;
    delete player2;
    logger->log("Borrado de jugadores finalizado.", DEBUG);   
    frontGroundTexture.free();
    middleGroundTexture.free();
    backGroundTexture.free();
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
	InputManager* inputManager = InputManager::getInstance();
    inputManager->update();
    if(inputManager->quitRequested()) m_Running = false;
}

void MCGame::update() {

	logger->log("Reubicacion inicio.", DEBUG);
	//distancia = player1->getPosX() + (198/2) - player2->getPosX() + (157/2);
	//distancia2 = player2->getPosX() + (157/2) - player1->getPosX() + (198/2);
	if (player1->getCentro() > player2->getCentro()) {
		distancia = player1->getPosX()+player1->getSobrante()+player1->getWidth() - (player2->getPosX()+player2->getSobrante());
		distancia2 = player2->getPosX()+player2->getSobrante() - (player1->getPosX()+player1->getSobrante()+player1->getWidth());
	} else {
		distancia = player1->getPosX()+player1->getSobrante() - (player2->getPosX()+player2->getSobrante()+player2->getWidth());
		distancia2 = player2->getPosX()+player2->getSobrante()+player2->getWidth() - (player1->getPosX()+player1->getSobrante());
	}
    logger->log("Actualizacion posicion MCGame.", DEBUG);
    player1->update(m_Renderer, distancia, player2->getCentro(),logger);
    player2->update(m_Renderer, distancia2, player1->getCentro(),logger);

    logger->log("Actualizacion parallax - MCGame.", DEBUG);
    parallaxController->doParallax(&player1,&player2,logger);
}

void orderBackgroundsByZIndex(json* backgroundList){

	int pos_sel = 0;
	json aux;

	for(int i = 2; i >= 0; i--){

		for(int x = 0; x <= i; x++){
			if(backgroundList[x]["zindex"] > backgroundList[pos_sel]["zindex"]){
				pos_sel = x;
			}
		}

		aux = backgroundList[i];
		backgroundList[i] = backgroundList[pos_sel];
		backgroundList[pos_sel] = aux;
		pos_sel = 0;
	}
}

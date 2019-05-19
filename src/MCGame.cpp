//
// Created by amaherok on 3/26/19.
//

#include "MCGame.h"
#include "Controls/WASDControls.h"
#include "Controls/ArrowControls.h"
#include <queue>
#include <thread>


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

                players[0]->loads(m_Renderer, 0);
                players[1]->loads(m_Renderer, 1);


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

	backGroundTexture.loadFromFile(backgroundFilepath, m_Renderer);
	middleGroundTexture.loadFromFile(middlegroundFilepath, m_Renderer);
	frontGroundTexture.loadFromFile(frontgroundFilepath, m_Renderer);

	backGround->setZIndex(backgroundsList[0]["zindex"]);
	middleGround->setZIndex(backgroundsList[1]["zindex"]);
	frontGround->setZIndex(backgroundsList[2]["zindex"]);
}



MCGame::MCGame(json config, int ancho, int alto, TCPClient* client) {

    this->logger = Logger::getInstance();
    this->SCREEN_WIDTH = ancho;
    this->SCREEN_HEIGHT = alto;
    this->tcpClient = client;
    m_Window = NULL;
    m_Renderer = NULL;
    m_Running = false;

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


    constants->spidermanSobrante = widthSpiderman * 242 / 640;
    constants->spidermanAncho = widthSpiderman * 110 / 640;
    constants->wolverineSobrante = widthWolverine * 278 / 640;
    constants->wolverineAncho = widthWolverine * 87 / 640;

    constants->INITIAL_POS_X_PLAYER_ONE = ((LEVEL_WIDTH / 2) - constants->spidermanSobrante) - (constants->spidermanAncho / 2) - 200;
    constants->INITIAL_POS_X_PLAYER_TWO = ((LEVEL_WIDTH / 2) - constants->wolverineSobrante) - (constants->wolverineAncho / 2) + 200;

    logger->log("Creacion de personajes.", DEBUG);

    //////////////////////////////////////////////////////////////////////

    // Setiar los characters con su numero de player segun server

    char* character1 = "Spiderman";
    char* character2 = "Wolverine";
    tcpClient->Send((void*) character1, sizeof(character1) + 1);
    tcpClient->Send((void*) character2, sizeof(character2) + 1);



    //Construyo los 4 personajes según la configuracion que me mande el server.

    for (int i = 0; i < 4; ++i) {
        void* buf1;
        character_builder_t* builder;
        tcpClient->socketClient->reciveData(buf1, sizeof(character_builder_t));
        builder = (character_builder_t*) buf1;
        characters[i] = characterBuild(builder);
    }

    char bufUpdater[sizeof(character_updater_t)];
    character_updater_t* up1;
    character_updater_t* up2;

    tcpClient->socketClient->reciveData(bufUpdater, sizeof(character_updater_t));
    up1 = (character_updater_t*) bufUpdater;
    cout << "Posicion X: " + to_string(up1->posX) << endl;
    cout << "Posicion Y: " + to_string(up1->posY) << endl;
    cout << "Current Action: "  + to_string(up1->action) << endl;
    cout << "Team: "  + to_string(up1->team) << endl;

    tcpClient->socketClient->reciveData(bufUpdater, sizeof(character_updater_t));
    up2 = (character_updater_t*) bufUpdater;
    cout << "Posicion X: " + to_string(up2->posX) << endl;
    cout << "Posicion Y: " + to_string(up2->posY) << endl;
    cout << "Current Action: "  + to_string(up2->action) << endl;
    cout << "Team: "  + to_string(up2->team) << endl;



    logger->log("Creacion de controles.", DEBUG);

    Controls* controlPlayer = new ArrowControls();

    this->clientControls = controlPlayer;

    players[0] = new Player(characters[0], characters[1]);

    players[1] = new Player(characters[2], characters[3]);


    logger->log("Creacion de Jugador.", DEBUG);


    logger->log("Definicion de Fondo.", DEBUG);

/*    characterBuilder_t characterBuilder;

    characterBuilder = tcpClient->receive(sizeof(characterBuilder_t));

    character_create(characterBuilder);
*/


    middleGround = new Layer(2400, 600, 3.33, 400); //3.33
    backGround = new Layer(1600,600,6.66667,800); //6.715
    frontGround = new Layer(3200,600,0,0);

    logger->log("Creacion de Parallax.", DEBUG);

    parallaxController = new Parallax(&middleGround, &backGround, &camera, &centerBefore, &centerLater, logger, SCREEN_WIDTH);
}

void MCGame::action_update() {
	actions_t lastAction = STANDING; //BORRAR
	bool threadRunning = true;
    while (true){
        handleEvents();
        actions_t actionToSend = clientControls->getNewAction();
        if(actionToSend == STANDING && lastAction == STANDING) //BORRAR
        	continue; //BORRAR
        tcpClient->socketClient->sendData(&actionToSend, sizeof(actionToSend));
        lastAction = actionToSend; //BORRAR
    }

}

void MCGame::run() {
	m_Running = true;
	FPSManager fpsManager(SCREEN_FPS);

	logger->log("Inicio de Bucle MCGame-run.", DEBUG);

	std::thread first (&MCGame::action_update, this);
    InputManager* inputManager = InputManager::getInstance();
    while(m_Running) {
		fpsManager.start();

		update();
		render();

		fpsManager.stop();
        if(inputManager->quitRequested()) {
            m_Running = false;
            first.detach();
        }
    }

	logger->log("Fin de Bucle MCGame-run.", DEBUG);
}

void orderRenderizableListByZIndex(Renderizable** list);

void MCGame::render() {
	logger->log("Inicio render.", DEBUG);
	SDL_SetRenderDrawColor( m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(m_Renderer); // clear the renderer to the draw color

	Renderizable* renderizables[5] = {  &(*middleGround), &(*backGround), &(*frontGround) , &(*players[0]) , &(*players[1])};
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
		else if(renderizables[i] == players[1]){
			players[1]->render(m_Renderer, camera.x, camera.y, players[0]->getCentro());
		}
		else if(renderizables[i] == players[0]){
			players[0]->render(m_Renderer, camera.x, camera.y, players[1]->getCentro());
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
}

//centro de 2 currentPlayers
//pos players[0], sobrante players[0], width players[0]
//pos players[1], sobrante players[1], width players[1]



void MCGame::update() {


    //tcpClient->recive()      // recibimos la struct de update
    //playersUpdate(structRecived)


    players[0]->update(m_Renderer, distancia, players[1]->getCentro());
//  players[1]->update(m_Renderer, distancia2, players[0]->getCentro());

    logger->log("Actualizacion parallax - MCGame.", DEBUG);


    // Mandamos all characters y lo hace con los que tienen playing = true;
    parallaxController->doParallax(&players[0],&players[1],logger);
}

CharacterClient *MCGame::characterBuild(character_builder_t *builder) {
   CharacterClient* characterClient = nullptr;


   switch(builder->personaje){
        case SPIDERMAN:
            if(builder->cliente < 2)
                characterClient = new SpidermanClient(constants->INITIAL_POS_X_PLAYER_ONE,
                                                      false,
                                                      constants->widthSpiderman,
                                                      constants->heightSpiderman,
                                                      constants->spidermanSobrante,
                                                      constants->spidermanAncho,
                                                      constants->screenWidth, builder->cliente);
            else
                characterClient = new SpidermanClient(constants->INITIAL_POS_X_PLAYER_TWO,
                                                      true,
                                                      constants->widthSpiderman,
                                                      constants->heightSpiderman,
                                                      constants->spidermanSobrante,
                                                      constants->spidermanAncho,
                                                      constants->screenWidth, builder->cliente);
            break;

        case WOLVERINE:
            if(builder->cliente < 2)
                characterClient = new WolverineClient(constants->INITIAL_POS_X_PLAYER_ONE,
                                                      false,
                                                      constants->widthWolverine,
                                                      constants->heightWolverine,
                                                      constants->wolverineSobrante,
                                                      constants->wolverineAncho,
                                                      constants->screenWidth, builder->cliente);
            else
                characterClient = new WolverineClient(constants->INITIAL_POS_X_PLAYER_ONE,
                                                      true,
                                                      constants->widthWolverine,
                                                      constants->heightWolverine,
                                                      constants->wolverineSobrante,
                                                      constants->wolverineAncho,
                                                      constants->screenWidth, builder->cliente);
    }
    return characterClient;
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




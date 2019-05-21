//
// Created by amaherok on 3/26/19.
//

#include "MCGame.h"
#include "Controls/WASDControls.h"
#include "Controls/ArrowControls.h"
#include <queue>
#include <thread>
#include "clienteMenu.h"


using namespace std;

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

const int SCREEN_FPS = 60;

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
    constants = (Constants*) (malloc(sizeof(Constants *)));
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

    constants->INITIAL_POS_X_PLAYER_ONE = ((LEVEL_WIDTH / 2) - constants->spidermanSobrante) - (constants->spidermanAncho / 2) - 200;
    constants->INITIAL_POS_X_PLAYER_TWO = ((LEVEL_WIDTH / 2) - constants->wolverineSobrante) - (constants->wolverineAncho / 2) + 200;

    logger->log("Creacion de personajes.", DEBUG);

    //////////////////////////////////////////////////////////////////////

    // MENU



    char* character1 = "Spiderman";
    char* character2 = "Wolverine";
    tcpClient->Send((void*) character1, sizeof(character1) + 1);
    tcpClient->Send((void*) character2, sizeof(character2) + 1);

    //MENU


    //Construyo los 4 personajes según la configuracion que me mande el server.

    for (int i = 0; i < 4; ++i) {
        void* buf1;
        character_builder_t* builder;
        buf1 = tcpClient->receive(sizeof(character_builder_t));
        builder = (character_builder_t*) buf1;
        characters[i] = characterBuild(builder);
    }


    logger->log("Creacion de controles.", DEBUG);

    Controls* controlPlayer = new ArrowControls();

    this->clientControls = controlPlayer;

    players[0] = new Player(characters[0], characters[1]);

    players[1] = new Player(characters[2], characters[3]);
    logger->log("Creacion de controles.", DEBUG);


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
    FPSManager fpsManager(25);


    while (true){
        fpsManager.start();

        handleEvents();
        if(!threadRunning)
            break;
        actions_t actionToSend = clientControls->getNewAction();
        tcpClient->socketClient->sendData(&actionToSend, sizeof(actionToSend));
        fpsManager.stop();

    }
    std::unique_lock<std::mutex> lock(m);
    m_Running = false;
}

void MCGame::run() {
	m_Running = true;
	FPSManager fpsManager(SCREEN_FPS);
	logger->log("Inicio de Bucle MCGame-run.", DEBUG);

	std::thread send (&MCGame::action_update, this);
	threadRunning=true;
    while(m_Running) {
		fpsManager.start();

		update();
		render();

		fpsManager.stop();
    }
    send.join();

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
	InputManager* inputManager = InputManager::getInstance();
    inputManager->update();
    if(inputManager->quitRequested()) threadRunning = false;
}

void MCGame::update() {

	char buf1[sizeof(character_updater_t)];
    tcpClient->socketClient->reciveData(buf1, sizeof(character_updater_t));
    character_updater_t* updater = (character_updater_t*) buf1;

    if(updater->team == 1) {
            players[0]->update(updater);
            players[0]->load(m_Renderer, players[1]->getCentro());
        }
    else{
            players[1]->update(updater);
            players[1]->load(m_Renderer, players[0]->getCentro());
        }

    logger->log("Actualizacion parallax - MCGame.", DEBUG);
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
            characterClient->setZIndex(constants->zIndexSpiderman);
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
            characterClient->setZIndex(constants->zIndexWolverine);
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




	character_builder_t* builder = (character_builder_t*) buf1;

	//this->updateNuevo(builder);
	//this->renderNuevo();

	//tiene que renderizar en base a lo que recibe en character_builder_t
	//Esto aca no anda porq no es de la clase, no entiendo si hay que updatear y renderizar en
	// el thread o afuera o encolar y despues updatear/renderizar



	return 0;
}

static void* clientWrite(void* args)
{
	TCPClient *client = (TCPClient*) args;



}

void MCGame::createReadThread()
{
	pthread_create( &(this->readThread), NULL, clientRead, this);
}

void MCGame::createWriteThread()
{
	pthread_create( &(this->writeThread),NULL, clientWrite, this);
}

void MCGame::renderNuevo()
{
	logger->log("Inicio render.", DEBUG);

	SDL_SetRenderDrawColor( m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(m_Renderer); // clear the renderer to the draw color

	Renderizable* renderizables[4] = { &(*middleGround), &(*backGround), &(*frontGround) , &(*characters[0])};
	orderRenderizableListByZIndex(renderizables);

	for(int i = 0; i < 4; i++){
		if(renderizables[i] == backGround){
			backGround->render(camera.x, camera.y, m_Renderer, &backGroundTexture, nullptr);
		}
		else if(renderizables[i] == middleGround){
			middleGround->render(camera.x, camera.y, m_Renderer, &middleGroundTexture, nullptr);
		}
		else if(renderizables[i] == frontGround){
			frontGround->render(0, 0, m_Renderer, &frontGroundTexture,&camera);
		}
		else if(renderizables[i] == characters[0]){
			characters[0]->render(m_Renderer, camera.x, camera.y, characters[0]->getCentro());
		}

	}
	logger->log("Fin render.", DEBUG);
    SDL_RenderPresent(m_Renderer); // draw to the screen
}

void MCGame::updateNuevo(render_data_t* render_data)
{
	logger->log("Reubicacion inicio.", DEBUG);

	if (render_data->currentCharacter1.centro > render_data->currentCharacter2.centro)
	{
		distancia = render_data->currentCharacter1.posX + render_data->currentCharacter1.sobrante +
					render_data->currentCharacter1.width -(render_data->currentCharacter2.posX +
					render_data->currentCharacter2.sobrante);

		distancia2 = render_data->currentCharacter2.posX + render_data->currentCharacter2.sobrante
					 -(render_data->currentCharacter1.posX + render_data->currentCharacter1.sobrante
					 +render_data->currentCharacter1.width);
	}
	else
	{
		distancia = render_data->currentCharacter1.posX + render_data->currentCharacter1.sobrante
					-(render_data->currentCharacter2.posX + render_data->currentCharacter2.sobrante
					+ render_data->currentCharacter2.width);

		distancia2 = render_data->currentCharacter2.posX + render_data->currentCharacter2.sobrante
					+ render_data->currentCharacter2.width - (render_data->currentCharacter1.posX
					+ render_data->currentCharacter1.sobrante);
	}
    logger->log("Actualizacion posicion MCGame.", DEBUG);

    //tcpClient->recive()      // recibimos la struct de update
    //playersUpdate(structRecived)

    //this->characters[0]->update(m_Renderer,distancia,render_data->currentCharacter2.centro)

    player1->update(m_Renderer, distancia, player2->getCentro());
//  player2->update(m_Renderer, distancia2, player1->getCentro());

    logger->log("Actualizacion parallax - MCGame.", DEBUG);


    // Mandamos all characters y lo hace con los que tienen playing = true;
    parallaxController->doParallax(&player1,&player2,logger);
}


/*Por ahora considero que se conectan 4 clientes (luego voy a considerar cuando se
 * conectan 2 o 3.
 * Es solo un boceto, ahora quito los booleanos y veo bien los datos a enviar al server*/
void MCGame::menu() {
	/* Posiblemente innecesario
	 * int numeroDeClientes; //Esto despues vuela
	switch(tcpClient->nclient)
	{
	   case 1:
	      numberTeam = 1;
	      break;
	   case 2:
		   if (numeroDeClientes==2) numberTeam = 2;
		   else numberTeam = 1;
		   break;
	   case 3:
		   numberTeam = 2;
	      break;
	   case 4:
		   numberTeam = 2;
	}*/


	bool terminar = false; //sto despues vuela (El servidor envia terminar cuando los dos team se bloquean (quedaron seleccionados todos los personajes)
	bool eligioASpiderman = true;
	bool teamBloqueado = false; //Esto despues vuela (es lo que voy a recibir del server)
	menuTexture.loadFromFile("images/menu/menu.png", m_Renderer);
	clienteMenu* cliente1 = new clienteMenu(97, 1);
	clienteMenu* cliente2 = new clienteMenu(449, 1);
	clienteMenu* cliente3 = new clienteMenu(97, 2);
	clienteMenu* cliente4 = new clienteMenu(449, 2);
	cliente1->load(m_Renderer,"images/menu/cliente1.png");
	cliente2->load(m_Renderer,"images/menu/cliente2.png");
	cliente3->load(m_Renderer,"images/menu/cliente3.png");
	cliente4->load(m_Renderer,"images/menu/cliente4.png");
	while (!terminar){
		InputManager* inputManager = InputManager::getInstance();
		inputManager->update();
		if (inputManager->isKeyDown(KEY_RIGHT)){
			//tcpClient->socketClient->sendData(&accion, sizeof(accion));	ARREGLAR
			eligioASpiderman = false;
		}
		if (inputManager->isKeyDown(KEY_LEFT)){
			//tcpClient->socketClient->sendData(&accion, sizeof(accion));	ARREGLAR
			eligioASpiderman = true;
		}
		if (inputManager->isKeyDown(KEY_RETURN)){
			//tcpClient->socketClient->sendData(&accion, sizeof(accion));	ARREGLAR
		}
		if(inputManager->quitRequested()) {
			terminar = true;
			m_Running = false;
		}
		//recibo la posicion en x de todos los clientes, si se bloqueo el equipo y si debo terminar 	ARREGLAR
		//tcpClient->socketClient->reciveData(m_d, sizeof(menu_data));
		//tcpClient->socketClient->reciveData(m_d, sizeof(menu_data));
		//tcpClient->socketClient->reciveData(m_d, sizeof(menu_data));
		//tcpClient->socketClient->reciveData(m_d, sizeof(menu_data));

		SDL_SetRenderDrawColor( m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(m_Renderer);
		menuTexture.render(0, 0, 800, 600, m_Renderer);

		/*cliente1->render(m_Renderer,posX);	ARREGLAR
		cliente2->render(m_Renderer,posX);
		cliente3->render(m_Renderer,posX);
		cliente4->render(m_Renderer,posX);*/
		SDL_RenderPresent(m_Renderer);
		}

		/*Esto ahora se encuentra en MCGame::MCGame(). Si llegase a funcionar se
		 * deberia quitar de ahi*/
		char* character1;
		char* character2;
		if (eligioASpiderman){
			character1 = "Spiderman";
			character2 = "Wolverine";
		} else {
			character1 = "Wolverine";
			character2 = "Spiderman";
		}
		/*Aprovecho que cuando elige un cliente, el otro cliente ya queda determinado,
		 * entonces un cliente de un equipo hace dos send y el otro cliente del mismo
		 * equipo no envia nada*/
		if (!teamBloqueado){
			tcpClient->Send((void*) character1, sizeof(character1) + 1);
			tcpClient->Send((void*) character2, sizeof(character2) + 1);
		}

}






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

                player1->loads(m_Renderer, 0);
                player2->loads(m_Renderer, 1);


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



    	//Crear wolverine

    //Seguir con el resto


    logger->log("Creacion de controles.", DEBUG);

    Controls* controlPlayer1 = new ArrowControls();

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

void MCGame::run() {
	m_Running = true;
	FPSManager fpsManager(SCREEN_FPS);

	logger->log("Inicio de Bucle MCGame-run.", DEBUG);


	//Se encarga de mandar al server los estados del personaje
	//thread->handle events()

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

	Renderizable* renderizables[4] = {  &(*middleGround), &(*backGround), &(*frontGround) , &(*characters[0])};
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
			player2->render(m_Renderer, camera.x, camera.y, player1->getCentro());
		}
		else if(renderizables[i] == player1){
			player1->render(m_Renderer, camera.x, camera.y, player2->getCentro());
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
    delete player1;
    delete player2;
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
    if(inputManager->quitRequested()) m_Running = false;
}

//centro de 2 currentPlayers
//pos player1, sobrante player1, width player1
//pos player2, sobrante player2, width player2



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

    //tcpClient->recive()      // recibimos la struct de update
    //playersUpdate(structRecived)


    player1->update(m_Renderer, distancia, player2->getCentro());
//  player2->update(m_Renderer, distancia2, player1->getCentro());

    logger->log("Actualizacion parallax - MCGame.", DEBUG);


    // Mandamos all characters y lo hace con los que tienen playing = true;
    parallaxController->doParallax(&player1,&player2,logger);
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

static void* clientRead(void* args)
{
	TCPClient *client = (TCPClient*) args;

	void* buf1[sizeof(character_builder_t)];

	client->socketClient->reciveData(&buf1, sizeof(character_builder_t));

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


/*Pensaba hacer que la distribución de personajes en los equipos sea por orden de
 * eleccion y no conexion. O sea, los primeros dos jugadores tienen total libertad
 * para elegir, entonces el primero que elige va al team1, y el segundo que elige
 * al team2. En ese punto se "bloquea" la pantalla, ya que los otros dos jugadores
 * que quedan en realidad ya no pueden elegir, sino que les toca el personaje que
 * no eligió el otro */
void MCGame::menu() {
	bool bloqueado = false;
	bool seleccionando = true;
	bool eligioASpiderman = true; //Por defecto arranca con Spiderman seleccionado
	menuTexture.loadFromFile("images/menu/menuS.png", m_Renderer);
	while (seleccionando && !bloqueado){
		//Aca falta recibir algo del server para saber si está bloqueado
		InputManager* inputManager = InputManager::getInstance();
		inputManager->update();
		if (inputManager->isKeyDown(KEY_RIGHT) && !bloqueado){
			menuTexture.loadFromFile("images/menu/menuW.png", m_Renderer);
			eligioASpiderman = false;
		}
		if (inputManager->isKeyDown(KEY_LEFT) && !bloqueado){
			menuTexture.loadFromFile("images/menu/menuS.png", m_Renderer);
			eligioASpiderman = true;
		}
		if(inputManager->quitRequested()) {
			seleccionando = false;
			m_Running = false;
		}
		SDL_SetRenderDrawColor( m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(m_Renderer);
		menuTexture.render(0, 0, 800, 600, m_Renderer);
		SDL_RenderPresent(m_Renderer);
		if (inputManager->isKeyDown(KEY_RETURN)) {
			seleccionando = false;
			bloqueado = true;
			//Aca tengo que enviar algo al server para que bloque
			//Tambien debo cargar la pantalla de bloqueo correspondiente
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
		if (!bloqueado){
			tcpClient->Send((void*) character1, sizeof(character1) + 1);
			tcpClient->Send((void*) character2, sizeof(character2) + 1);
		}

	}
}






//
// Created by amaherok on 3/26/19.
//

#include "../Personajes/Wolverine.h"
#include "MCGame.h"

using namespace std;

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

//Scene textures

Texture g_BackgroundTexture;
Wolverine wolverine;
Spiderman spiderman;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int distancia;
int distancia2;

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

bool MCGame::init(const char *title, int xpos, int ypos, int width, int height, int flags) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

    	this->logger->log("SDL no se pudo inicializar.", ERROR);
        return false;

    } else {
    	this->logger->log("SDL Inicializó correctamente.", INFO);

        m_Window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (m_Window == 0) {

        	this->logger->log("Fallo al crear ventana.", ERROR);
            return false;

        } else {
        	this->logger->log("Ventana creada exitosamente.", INFO);

            m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);
            if (m_Renderer == 0) {
            	this->logger->log("Fallo al crear Renderer", ERROR);

                return false;
            } else {
            	this->logger->log("Renderer creado exitosamente.", INFO);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                	this->logger->log("SDL_image no pudo inicializarse.", ERROR);
                    return false;
                }

                spiderman.spidermanLoad(m_Renderer);
                wolverine.wolverineLoad(m_Renderer);
                g_BackgroundTexture.loadFromFile("images/camino.png", m_Renderer);
            }
        }
    }

    m_Running = true; // everything inited successfully,
    return true;
}

MCGame::MCGame(Logger* log){
	this->logger = log;
	this->m_Window = NULL;
	this->m_Renderer = NULL;
	this->m_destinationRectangle = NULL;
	this->m_sourceRectangle = NULL;
	this->m_Running = false;
}

void MCGame::render() {
	SDL_SetRenderDrawColor( m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(m_Renderer); // clear the renderer to the draw color
    g_BackgroundTexture.render(0, 0, m_Renderer, &camera);
    wolverine.render(m_Renderer,camera.x , camera.y);
    spiderman.render(m_Renderer, camera.x, camera.y);
    SDL_RenderPresent(m_Renderer); // draw to the screen
}

void MCGame::clean() {
    //m_Texture.free();
    spiderman.free();
    g_BackgroundTexture.free();
    std::cout << "cleaning game\n";
    SDL_DestroyWindow(m_Window);
    SDL_DestroyRenderer(m_Renderer);
    SDL_Quit();
}

void MCGame::handleEvents() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_Running = false;
                break;
            default:
                break;
        }
        distancia = spiderman.getPosX()-wolverine.getPosX();
        distancia2 = wolverine.getPosX()-spiderman.getPosX();
        wolverine.handleEvent(event, m_Renderer, distancia2);
        spiderman.handleEvent(event, m_Renderer, distancia);
    }
    //Center the camera over the dot
    camera.x = (((( spiderman.getPosX() + Spiderman::DOT_WIDTH / 2 ) + ( wolverine.getPosX() + Wolverine::DOT_WIDTH / 2 )) / 2 ) - SCREEN_WIDTH / 2)+350;
    camera.y = ( spiderman.getPosY() + Spiderman::DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds
    if ( (camera.x) < 0 )
    {
    	camera.x = 0;
    }
    if ( (camera.y) < 0 )
    {
        camera.y = 0;
    }
    if ( (camera.x) > 3200 - (camera.w) )
    {
        camera.x = 3200 - camera.w;
    }
    if ( (camera.y) > 600 - (camera.h) )
    {
        camera.y = 600 - camera.h;
    }

}

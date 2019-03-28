//
// Created by amaherok on 3/26/19.
//

#include "headers/MCGame.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include "LTexture.h"

using namespace std;



//Scene textures

LTexture g_BackgroundTexture;
LTexture g_Wolverine;

bool MCGame::init(const char *title, int xpos, int ypos, int width, int height, int flags) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL no se pudo inicializar");
        return false;
    } else {
        m_Window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (m_Window == 0) {
            printf("Fallo al crear ventana");
            return false;
        } else {
            m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);
            if (m_Renderer == 0) {
                printf("Fallo al crear Renderer");
                return false;
            } else {
                SDL_SetRenderDrawColor(m_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    // printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    return false;
                }
                m_Texture.loadFromFile("images/SpiderMan397.png", m_Renderer);
                g_Wolverine.loadFromFile("images/Wolverine_373.png", m_Renderer);
                g_BackgroundTexture.loadFromFile("images/background.png", m_Renderer);
            }
        }
    }
    std::cout << "init success\n";
    m_Running = true; // everything inited successfully,
    return true;
}


void MCGame::render() {
    SDL_RenderClear(m_Renderer); // clear the renderer to the draw color
    g_BackgroundTexture.render(0, 0, m_Renderer);
    g_Wolverine.render(-100,-40,m_Renderer);
    m_Texture.render(240, 0, m_Renderer);
    SDL_RenderPresent(m_Renderer); // draw to the screen
}

void MCGame::clean() {
    m_Texture.free();
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
    }
}
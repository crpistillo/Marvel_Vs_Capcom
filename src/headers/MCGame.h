//
// Created by amaherok on 3/26/19.
//

#ifndef MARVEL_VS_CAPCOM_MCGAME_H
#define MARVEL_VS_CAPCOM_MCGAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

using namespace std;


class MCGame {
private:
    bool m_Running;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    SDL_Texture* m_Texture; // the new SDL_Texture variable
    SDL_Rect* m_sourceRectangle; // the first rectangle
    SDL_Rect* m_destinationRectangle; // another rectangle

public:
    MCGame(){}
    ~MCGame() {}
    void init() { m_Running = true; }
    bool init(const char* title, int xpos, int ypos, int width, int
    height, int flags);
    void render();
    void update(){}
    void handleEvents();
    void clean();
    // a function to access the private running variable
    bool running() {
        return m_Running;
    }


};


#endif //MARVEL_VS_CAPCOM_MCGAME_H

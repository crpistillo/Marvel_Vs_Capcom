//
// Created by IgVelasco on 6/20/19.
//

#include "TimeBanner.h"
const string timeFilePath = "images/numeros_y_barras/";
const string FILE_EXTENSION = ".png";

TimeBanner::TimeBanner(int posX) {
    loader = new ImageLoader((Uint8) 255, (Uint8) 255, (Uint8) 255);
    digit = 9;
    position = posX;
}

void TimeBanner::load(SDL_Renderer *m_Renderer) {
    switch(digit) {
        case 0:
            this->loader->loadActionSprite(timeFilePath, "", 0, FILE_EXTENSION, m_Renderer, &texture);
            break;
        case 1:
            this->loader->loadActionSprite(timeFilePath, "", 1, FILE_EXTENSION, m_Renderer, &texture);
            break;
        case 2:
            this->loader->loadActionSprite(timeFilePath, "", 2, FILE_EXTENSION, m_Renderer, &texture);
            break;
        case 3:
            this->loader->loadActionSprite(timeFilePath, "", 3, FILE_EXTENSION, m_Renderer, &texture);
            break;
        case 4:
            this->loader->loadActionSprite(timeFilePath, "", 4, FILE_EXTENSION, m_Renderer, &texture);
            break;
        case 5:
            this->loader->loadActionSprite(timeFilePath, "", 5, FILE_EXTENSION, m_Renderer, &texture);
            break;
        case 6:
            this->loader->loadActionSprite(timeFilePath, "", 6, FILE_EXTENSION, m_Renderer, &texture);
            break;
        case 7:
            this->loader->loadActionSprite(timeFilePath, "", 7, FILE_EXTENSION, m_Renderer, &texture);
            break;
        case 8:
            this->loader->loadActionSprite(timeFilePath, "", 8, FILE_EXTENSION,m_Renderer, &texture);
            break;
        case 9:
            this->loader->loadActionSprite(timeFilePath, "", 9, FILE_EXTENSION,m_Renderer, &texture);
            break;
    }
}

void TimeBanner::render(SDL_Renderer *mRenderer) {
    texture.render(300, 0, 50, 50 ,mRenderer);
}


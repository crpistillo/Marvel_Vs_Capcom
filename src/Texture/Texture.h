//
// Created by amaherok on 3/26/19.
//

#ifndef MARVEL_VS_CAPCOM_TEXTURE_H
#define MARVEL_VS_CAPCOM_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>


class Texture {
public:
    //Initializes variables
    Texture();

    //Deallocates memory
    ~Texture();

    //Loads image at specified path
    bool loadFromFile(std::string path, SDL_Renderer *mRenderer);

    //Deallocates texture
    void free();

    //Renders texture at given point
    void render(int x, int y, SDL_Renderer *mRenderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //Gets image dimensions
    int getWidth();

    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture *mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};


#endif //MARVEL_VS_CAPCOM_TEXTURE_H

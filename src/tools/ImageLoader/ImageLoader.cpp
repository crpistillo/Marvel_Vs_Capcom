#include "ImageLoader.h"


ImageLoader::ImageLoader(){}
ImageLoader::~ImageLoader(){}

void ImageLoader::loadActionSprite(string actionPath, string characterMVC, int currentSprite, string extension, SDL_Renderer *renderer, Texture* texture){

	string imagePath = actionPath + characterMVC + to_string(currentSprite) + extension;

	texture->loadFromFile(imagePath, renderer);
}

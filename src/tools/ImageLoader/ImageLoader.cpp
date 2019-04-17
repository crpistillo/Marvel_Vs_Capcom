#include "ImageLoader.h"


ImageLoader::ImageLoader(unsigned int r, unsigned int g, unsigned int b){
	this->r_value = r;
	this->g_value = g;
	this->b_value = b;
}
ImageLoader::~ImageLoader(){}

void ImageLoader::loadActionSprite(string actionPath, string characterMVC, int currentSprite, string extension, SDL_Renderer *renderer, Texture* texture){

	string imagePath = actionPath + characterMVC + to_string(currentSprite) + extension;

	texture->loadFromFile(imagePath, renderer);
	texture->setColor(r_value, g_value, b_value);
}

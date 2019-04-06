#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include "../../Texture/Texture.h"

using namespace std;


//A personal Image Loader for each character.
class ImageLoader{

public:
	ImageLoader();
	~ImageLoader();

	void loadActionSprite(string actionPath, string characterMVC, int currentSprite, string extension, SDL_Renderer *renderer, Texture* texture);

};





#ifndef CLIENTCURSOR_H_
#define CLIENTCURSOR_H_

#include "./Texture/Texture.h"
#include "data_structs.h"

class ClientCursor {
public:


	ClientCursor(int posX, int posY, SDL_Renderer *renderer, bool isVisible);

    //void load(SDL_Renderer* renderer, string path);
    void render(SDL_Renderer* mRenderer);

    /*void moveRight(SDL_Renderer *renderer);
    void moveLeft(SDL_Renderer *renderer);*/

	void update(cursor_updater_t* updater);
    bool visible;

private:
    int posX, posY;
    int clientRepresenting;
    bool finalSelection;

    Texture cursorTexture;
};



#endif /* CLIENTCURSOR_H_ */

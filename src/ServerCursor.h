#ifndef SERVER_CURSOR_H
#define SERVER_CURSOR_H

#include "data_structs.h"


class ServerCursor{
public:

	ServerCursor(int posX, int posY);

	void moveRight();
	void moveLeft();


private:

	int posX, posY;
	bool finalSelection;
	character_number_t characterSelected;


};


#endif /*SERVER_CURSOR_H*/

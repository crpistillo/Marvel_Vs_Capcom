#ifndef SERVER_CURSOR_H
#define SERVER_CURSOR_H

#include "data_structs.h"
#include <iostream>

class ServerCursor{
public:

	ServerCursor(int posX, int posY);

	void moveRight();
	void moveLeft();

	void update(cliente_menu_t* action_msg);


private:

	int posX, posY;
	bool finalSelection;
	character_number_t characterSelected;


};


#endif /*SERVER_CURSOR_H*/

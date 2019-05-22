#ifndef SERVER_CURSOR_H
#define SERVER_CURSOR_H

#include "data_structs.h"
#include <iostream>

class ServerCursor{
public:

	ServerCursor(int posX, int posY);

	bool moveRight();
	bool moveLeft();

	bool update(cliente_menu_t* action_msg);

	void makeMenuUpdater(cursor_updater_t* updater);


private:

	int posX, posY;
	bool finalSelection;
	character_number_t characterSelected;


};


#endif /*SERVER_CURSOR_H*/

#ifndef MVC_DATA_STRUCTS
#define MVC_DATA_STRUCTS


#include "action_constants.h"
#include <string>

typedef enum connection_status{
	NOT_READY = 0,
	READY = 1,
	NO_MORE_CONNECTIONS_ALLOWED = 2
} connection_status_t;

typedef struct{
	connection_status_t status;
	int nconnections;
} connection_information_t;

typedef struct{
	int cliente;
	char* personaje1;
	char* personaje2;
} prueba_t;

typedef enum character_number{
	NOT_VALID = -1,
	SPIDERMAN = 0,
	WOLVERINE = 1
} character_number_t;


typedef struct{
	int pos;
    int cliente;
    character_number_t personaje;
    int sprite;
    actions_t action;
    bool isFirstTeam;
} character_builder_t;

typedef struct{
	int posX;
	int posY;
	int currentSprite;
	actions_t action;
	int team;
	int client;
} character_updater_t;


//centro de 2 currentPlayers
//pos player1, sobrante player1, width player1
//pos player2, sobrante player2, width player2


typedef struct
{
	int centro;
	int posX;
	int sobrante;
	int width;
}character_render_data_t;

typedef struct
{
	character_render_data_t currentCharacter1;
	character_render_data_t currentCharacter2;
}render_data_t;

typedef struct
{
	int client;
	actions_t action;
}incoming_msg_t;

typedef struct {
	int cliente;
	menu_action_t accion;
} cliente_menu_t;

typedef struct {
	int cliente;
	int posX;
	int posY;
	bool finalSelection;
	bool menuTerminated;
	bool visible;
} cursor_updater_t;


typedef struct {
	std::string ip;
	bool isActive;
} ip_status_t;



typedef enum game_instance{
	BEGINNING = 9,
	MENU_PHASE = 10,
	FIGHT_PHASE = 11,
} game_instance_t;

typedef struct {
    game_instance_t instance;
    int client;
    int players;

} initializer_t;





#endif /* MVC_DATA_STRUCTS */

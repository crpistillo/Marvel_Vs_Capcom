#ifndef MVC_DATA_STRUCTS
#define MVC_DATA_STRUCTS


#include "orientation_constants.h"

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
	SPIDERMAN = 0,
	WOLVERINE = 1
} character_number_t;

typedef struct{
    int cliente;
    character_number_t personaje;
    int sprite;
    actions_t action;
} character_builder_t;


#endif /* MVC_DATA_STRUCTS */

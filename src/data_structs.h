#ifndef MVC_DATA_STRUCTS
#define MVC_DATA_STRUCTS


typedef enum connection_status{
	NOT_READY = 0,
	READY = 1,
	NO_MORE_CONNECTIONS_ALLOWED = 2
} connection_status_t;

typedef struct{
	connection_status_t status;
	int nconnections;
} connection_information_t;

#endif /* MVC_DATA_STRUCTS */

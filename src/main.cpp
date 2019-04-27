#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cliente.h"
#include "Server.h"

int main(int argc, char* argv[]) {
	if (strncmp(argv[2],"client",6)==0) {
	//	validar_param_cliente(argv);
		iniciar_cliente(argc,argv[1],argv[2],argv[3],argv[4]);
	} else if (strncmp(argv[2],"server",6)==0){
		//validar_param_servidor(*argv);
	//	iniciar_servidor((int)argv[2]);
	//int puerto=8080;
	iniciar_servidor(argv[4]);
	} else { printf ("Parametro ingresado Incorrecto\n");
			return 1;}

    return 0;
}

/* Parametros de inicio:
 * 1) Archivo json
 * 2) Tipo de ejecucion (cliente o servidor)
 * 3) Ip
 * 4) Puerto
 *
 */

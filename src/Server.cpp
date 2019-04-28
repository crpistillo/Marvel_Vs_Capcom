#include <stdio.h>
#include "Server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int initServer(char* port) {
    int Descriptor;
    char buf[1024];
    char enviar[1024];
	struct sockaddr_in Direccion;
	Direccion.sin_family = AF_INET;
	Direccion.sin_port = htons(8080); //	htons((int)port);
	Direccion.sin_addr.s_addr = INADDR_ANY;

	Descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if(Descriptor == -1) {
		printf("Error en apertura de socket servidor\n");
    } else {
		if(bind(Descriptor,(struct sockaddr *)&Direccion, sizeof (Direccion)) == -1) {
			printf ("Error en apertura de conexion\n");
		} else {
			listen(Descriptor,4);
			socklen_t  sin_size=sizeof(Direccion);
			int new_sock = accept(Descriptor,(struct sockaddr *)&Direccion,&sin_size);
			if(new_sock == -1){
				printf("Error en aceptacion de conexion\n");
			}
			//Ciclo para enviar y recibir mensajes
			 while(1) {
				 //El servidor espera el primer mensaje
				 recv(new_sock,buf,sizeof(buf),0);
				 if(strcmp(buf, "salir") == 0) {
					 break;
				 }
				 printf("Cliente: %s\n", buf);

				 //El cliente recibe el mensaje del servidor
				 printf("Escribir mensaje: ");
				 scanf("%s[^\n]", &enviar);

				 send(new_sock,enviar,sizeof(enviar),0);

				 if(strcmp(enviar,"salir")==0){
					 break;
				 }
			 }
		}
	}

	close(Descriptor);
    return 0;
}

#include <stdio.h>
#include "Server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main(int argc, char** argv) {
    int Descriptor;
    char Datos[]="Pepe";
    int continua;
    char buf[1024];
    char enviar[1024];
    char enviar2[1024];
	struct sockaddr_in Direccion;
	Direccion.sin_family = AF_INET;
	Direccion.sin_port = htons(8080); //	htons((int)port);
	Direccion.sin_addr.s_addr = INADDR_ANY;

	Descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (Descriptor == -1) {
		printf ("Error en apertura de socket servidor\n");
    }else{
		if(bind(Descriptor,(struct sockaddr *)&Direccion, sizeof (Direccion)) == -1){
			printf ("Error en apertura de conexion\n");
		}else{
			listen(Descriptor,4);
	socklen_t  sin_size=sizeof(Direccion);
	int new_sock = accept(Descriptor,(struct sockaddr *)&Direccion,&sin_size);
	if(new_sock == -1){
		printf ("Error en aceptacion de conexion\n");

	}
	//read(new_sock,Datos,4);


	//Ciclo para enviar y recibir mensajes
	 while(1){
	 //El servidor espera el primer mensaje
	 recv(new_sock,buf,1024,0);
	 //if(strcmp(&buf,"salir")==0){
//	 break;
	// }
	 printf("Cliente: %s\n",buf);

	 //El cliente recibe el mensaje del servidor
	 printf("Escribir mensaje: ");
	 scanf("%*c%[^\n]",enviar2);
	 send(new_sock,enviar2,1024,0);
	 if(strcmp(enviar2,"salir")==0){
	 break;
	 }
	}

			//printf("MSG: %s\n",Datos);

		}
	}
	close(Descriptor);

    return 0;
}

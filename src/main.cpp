//============================================================================
// Name        : main.cpp
// Author      : Grupo Capcom
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "tools/json/ConfigFileParser/ConfigFileParser.h"
#include "tools/logger/Logger.h"
#include "MCGame.h"
#include "Server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


#define DATA "Hola"

const string ERROR = "ERROR";
const string INFO = "INFO";
const string DEBUG = "DEBUG";

using namespace std;

json parseConfigFile(string logPath) {
	ConfigFileParser* parser = new ConfigFileParser(logPath);
	parser->parse();
	json config = parser->getConfig();
	delete parser;
	return config;
}

MCGame* mcGame = 0;
Logger *Logger::instance = 0;

int main(int argc, char** argv) {
	Logger* logger = Logger::getInstance();
	logger->startSession();
	logger->log("Logger iniciado.", DEBUG);
	json config;

    if(argc != 2){
    	logger->log("Archivo de configuracion no especificado.", INFO);
    	config = parseConfigFile("config/config_default.json");
    }
    else{
    	string configPath = (string) argv[1];
    	logger->log("Procediento a utilizar archivo de configuracion especificado: " + configPath , INFO);
    	config = parseConfigFile(configPath);
    }
	int ancho = config["window"]["width"];
	int alto = config["window"]["height"];
	logger->log("Configuracion Cargada - Inicio de Ciclo.", INFO);
////////////////
    char buf[1024];
    char enviar[1024];
	char Datos[] = "Hola";
    int Descriptor;
    char* mensaje;
    int continua;
	struct sockaddr_in Direccion;
	Direccion.sin_family = AF_INET;
	Direccion.sin_port = htons(8080); //	htons((int)port);
	Direccion.sin_addr.s_addr = INADDR_ANY;

	printf("MSG: %s\n",Datos);

	Descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (Descriptor == -1) {
		printf ("Error en apertura de socket servidor\n");
    }else{
    int estado_conexion = connect(Descriptor,(struct sockaddr *)&Direccion, sizeof (Direccion));
	if (estado_conexion == -1) {
		printf("Hubo un error en la conexion remota\n");
	}else{
		//send(Descriptor, DATA, sizeof(DATA),0);
//		write(Descriptor,Datos,4);
		//printf("llego al cliente");
	while(1){
	 //El servidor espera el primer mensaje
	 printf("Escribir mensaje: ");
	 scanf("%*c%[^\n]",enviar);
	 send(Descriptor,enviar,1024,0);
	 if(strcmp(enviar,"salir")==0){
	 break;
	 }

	 //El cliente recibe el mensaje del servidor
	 recv(Descriptor,buf,1024,0);
	 if(strcmp(buf,"salir")==0){
	 break;
	 }
	 printf("Servidor: %s\n",buf);
	}

	close(Descriptor);

}
}
	//////////////////////
    mcGame = new MCGame(config, ancho, alto);
    mcGame->camera = { 0, 0, ancho, alto };
    mcGame->init("Marvel vs Capcom", 100, 100, ancho, alto, 0);
    mcGame->run();

    mcGame->clean();
    logger->log("Fin de ciclo de ejecucion.", INFO);
    logger->finishSession();
    delete logger;
    delete mcGame;
    return 0;
}

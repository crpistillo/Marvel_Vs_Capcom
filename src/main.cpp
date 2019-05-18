#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tools/json/ConfigFileParser/ConfigFileParser.h"
#include "tools/logger/Logger.h"
#include "Client/TCPClient.h"
#include "TCPServer.h"
#include "tools/logger/Logger.h"
#include "MCGame.h"
#include "ServerThread.h"
#include "data_structs.h"
#include <sys/socket.h>

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

Logger *Logger::instance = 0;

TCPServer* tcpServer;
TCPClient* tcpClient;
MCGame* mcGame;


int run_server(int cantArg, char *dirJson, int port);
int run_client(int cantArg, char *dirJson, string host, int port);


int main(int argc, char *argv[]) {

    if (strncmp(argv[2], "client", 6) == 0) {
        run_client(argc, argv[1], string(argv[3]),atoi(argv[4]));
    } else {
        if (strncmp(argv[2], "server", 6) == 0) {
           return run_server(argc, argv[1], atoi(argv[3]));
        } else {
            printf("Parametro ingresado Incorrecto\n");
            return 1;
        }
    }

    return 0;
}

/* Parametros de inicio client:               |           *Parametros de inicio server:
 * 1) Archivo json                            |           *1) Archivo json
 * 2) Tipo de ejecucion (cliente o servidor)  |           *2) Tipo de ejecucion (cliente o servidor)
 * 3) Ip                                      |           *3) Puerto
 * 4) Puerto                                  |
 *
 */


int run_server(int cantArg, char *dirJson, int port) {

	Logger* logger = Logger::getInstance();
	logger->startSession("SERVER");
	tcpServer = new TCPServer();

	logger->log("Logger iniciado.", INFO);
	json config;

	if(cantArg != 4){
		logger->log("Cantidad de parametros necesarios es incorrecto. Deben ser 4.", ERROR);
		logger->finishSession();
		return -1;
	}

	FILE *configFile;
    if( !(configFile = fopen( dirJson,  "r")) ){ //VVerifico que el archivo que me pasan exista.
    	logger->log("Archivo de configuracion no existente, cargando el archivo de configuracion por defecto.", INFO);
	    config = parseConfigFile("config/config_default.json");
    }
    else{
    	fclose(configFile);
        string configPath = (string) dirJson;
        logger->log("Procediento a utilizar archivo de configuracion especificado: " + configPath , INFO);
	    config = parseConfigFile(dirJson);
    }

    //ServerThread* serverThread = new ServerThread(tcpServer);  //No le veo utilidad a esta clase, por ahora.

    if(!tcpServer->setup(port, logger)){
        cout << "Error al crear el server" << endl;
        return -1;
    }

    /*if (serverThread->create()) {
        tcpServer->receive();
    }*/
    //tcpServer->receive(logger);

    if( tcpServer->createAceptingThread() )   //Crea el thread que escucha conexiones nuevas.
    	return -1;

    while( tcpServer->getNumberOfConections() != MAXPLAYERS)	//Espera hasta que se conecten MAXPLAYERS
    	continue;

    cout << "Numero de jugadores alcanzado! \n";

    tcpServer->createReceivingThreadPerClient();
    tcpServer->createSendingThreadPerClient();

    logger->finishSession();

    while(1)							//Halt. Aca la aplicacion deberia seguir haciendo otra cosa.
    	continue;

    exit(0);
}

int run_client(int cantArg, char *dirJson, string host, int port) {

	Logger* logger = Logger::getInstance();
	logger->startSession("CLIENT");

	tcpClient = new TCPClient();

	//LOGGER//////////
    logger->log("Logger iniciado.", DEBUG);
    json config;

    if(cantArg != 2){
        logger->log("Archivo de configuracion no especificado, cargando el archivo de configuracion por defecto.", INFO);
        config = parseConfigFile("config/config_default.json");
    }
    else{
        string configPath = (string) dirJson;
        logger->log("Procediento a utilizar archivo de configuracion especificado: " + configPath , INFO);
        config = parseConfigFile(dirJson);
    }
    int ancho = config["window"]["width"];
    int alto = config["window"]["height"];
    logger->log("Configuracion Cargada - Inicio de Ciclo.", INFO);

    /////////////////

    if(!tcpClient->setup(host, port)) {                  //MCGame tendria que tener el tcpClient
        cout << "Failed to setup Client" << endl;
        return -1;
    }
//    tcpClient->Send("Connection succesfull");

    void* msj;
    while(1){
    	msj = tcpClient->receive(sizeof(connection_information_t));
    	connection_information_t* buf = (connection_information_t*) msj;
    	if(buf->status == NO_MORE_CONNECTIONS_ALLOWED){
    		cout << "Connection not allowed. No more players. \n";
    		return 0;
    	}

    	if(buf->status == READY)
    		break;
    	else{
    		tcpClient->nclient = buf->nconnections;
    		cout << "Not ready to launch. Players: " + to_string(buf->nconnections) + "/2\n";
    	}
    }

    mcGame = new MCGame(config, ancho, alto, tcpClient);
    mcGame->camera = { 0, 0, ancho, alto };
    mcGame->init("Marvel vs Capcom", 100, 100, ancho, alto, 0);
    mcGame->run();


    return 0;
}



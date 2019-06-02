#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tools/json/ConfigFileParser/ConfigFileParser.h"
#include "tools/logger/Logger.h"
#include "Client/TCPClient.h"
#include "TCPServer.h"
#include "tools/logger/Logger.h"
#include "MCGame.h"
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


int run_server(int cantArg, char *dirJson, int port, int numOfPlayers);
int run_client(int cantArg, char *dirJson, string host, int port);


int main(int argc, char *argv[]) {

    if (strncmp(argv[2], "client", 6) == 0) {
        run_client(argc, argv[1], string(argv[3]),atoi(argv[4])); //le estoy mandando de a cuantos armo el server esto tiene que ir en el json
    } else {
        if (strncmp(argv[2], "server", 6) == 0) {
           return run_server(argc, argv[1], atoi(argv[3]), atoi(argv[4]));
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


int run_server(int cantArg, char *dirJson, int port, int numOfPlayers) {

	Logger* logger = Logger::getInstance();
	logger->startSession("SERVER");
	tcpServer = new TCPServer();

	logger->log("Logger iniciado.", INFO);
	json config;

	if(cantArg != 5){
		logger->log("Cantidad de parametros necesarios es incorrecto. Deben ser 5.", ERROR);
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

    if(!tcpServer->setup(port, logger, numOfPlayers)){
        cout << "Error al crear el server" << endl;
        return -1;
    }

    tcpServer->configJson(config);
    tcpServer->runServer();

    logger->finishSession();

  /*  while(1) {

    }*/
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

    tcpClient->ancho = ancho;
    tcpClient->alto = alto;
    tcpClient->config = config;

    if(!tcpClient->setup(host, port)) {                  //MCGame tendria que tener el tcpClient
        cout << "Failed to setup Client" << endl;
        return -1;
    }

    tcpClient->run();


    return 0;
}



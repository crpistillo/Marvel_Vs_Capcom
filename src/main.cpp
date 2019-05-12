#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tools/json/ConfigFileParser/ConfigFileParser.h"
#include "tools/logger/Logger.h"
#include "Client/TCPClient.h"
#include "TCPServer.h"
#include "tools/logger/Logger.h"
#include "MCGame.h"

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


int run_server(int cantArg, char *dirJson, int port, Logger* logger);
int run_client(int cantArg, char *dirJson, string host, int port, Logger* logger);
void *loop(void *m);


int main(int argc, char *argv[]) {
	Logger* logger = Logger::getInstance();

    if (strncmp(argv[2], "client", 6) == 0) {
        run_client(argc, argv[1], string(argv[3]),atoi(argv[4]), logger);
    } else {
        if (strncmp(argv[2], "server", 6) == 0) {
           return run_server(argc, argv[1], atoi(argv[3]),logger);
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


int run_server(int cantArg, char *dirJson, int port, Logger* logger) {

	tcpServer = new TCPServer();

	logger->startSession();
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


    pthread_t msg;

    if(!tcpServer->setup(port, logger)){
        cout << "Error al crear el server" << endl;
        return -1;
    }
    if (pthread_create(&msg, NULL, loop, NULL) == 0) {
        tcpServer->receive();
    }
    return 0;
}

int run_client(int cantArg, char *dirJson, string host, int port, Logger* logger) {

	tcpClient = new TCPClient();

    logger->startSession();
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

    if(!tcpClient->setup(host, port)) {                  //MCGame tendria que tener el tcpClient
        cout << "Failed to setup Client" << endl;
        return -1;
    }
    tcpClient->Send("Connection succesfull");
    /*while (1){
        string str = tcpClient.receive(17);
        cout << str <<  endl;
        if(str == "Connection ready")
            break;
    }*/

    mcGame = new MCGame(config, ancho, alto, tcpClient);
    mcGame->camera = { 0, 0, ancho, alto };
    mcGame->init("Marvel vs Capcom", 100, 100, ancho, alto, 0);
    mcGame->run();


    return 0;
}



void *loop(void *m) {
    pthread_detach(pthread_self());
    while (1) {
        string str = tcpServer->getMessage();
        if (!str.empty()) {
            cout << "Message:" << str << endl;
            tcpServer->Send("Connection ready");
            if(str == "salir")
                break;
            tcpServer->clean();
        }
    }
    tcpServer->detach();
}


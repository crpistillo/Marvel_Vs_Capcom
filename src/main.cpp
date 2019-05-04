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

TCPServer tcpServer;
TCPClient tcpClient;
MCGame* mcGame;


int run_server(int port);
int run_client(int cantArg, char *dirJson, string host, int port);

int main(int argc, char *argv[]) {
    if (strncmp(argv[2], "client", 6) == 0) {
        //validateClientParams(argv);
        // initClient(argc, argv[1], string(argv[3]), atoi(argv[4]));
        run_client(argc, argv[1], string(argv[3]),atoi(argv[4]));
    } else {
        if (strncmp(argv[2], "server", 6) == 0) {
            run_server(atoi(argv[3]));
        } else {
            printf("Parametro ingresado Incorrecto\n");
            return 1;
        }
    }

    return 0;
}



void *loop(void *m) {
    pthread_detach(pthread_self());
    while (1) {
        char ch[1024];
        string str = tcpServer.getMessage();
        if (!str.empty()) {
            cout << "Message:" << str << endl;
            if(str == "salir")
                break;
            /*   cout << "Enviar Mensaje:" << endl;

               scanf("%s \n", ch);
               string s = string(ch);
               tcpServer.Send(" [client message: " + str + "] " + s);
               */
            tcpServer.clean();
        }
    }
    tcpServer.detach();
}


int run_server(int port) {
    pthread_t msg;
    tcpServer.setup(port);
    if (pthread_create(&msg, NULL, loop, (void *) 0) == 0) {
        tcpServer.receive();
    }
    return 0;
}

int run_client(int cantArg, char *dirJson, string host, int port) {

    Logger* logger = Logger::getInstance();
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

    tcpClient.setup(host, port);             //MCGame tendria que tener el tcpClient
    tcpClient.Send("Connection succesfull");

    mcGame = new MCGame(config, ancho, alto);
    mcGame->camera = { 0, 0, ancho, alto };
    mcGame->init("Marvel vs Capcom", 100, 100, ancho, alto, 0);
    mcGame->run();


    /*while(1)
    {
        printf("Enviar Mensaje:");
        scanf("%s", buf);

        tcpClient.Send(string(buf));
        if(string(buf) == "salir")
            break;

        string rec = tcpClient.receive();
        if( rec != "" )
        {
            cout << "Server Response:" << rec << endl;
            if(rec == "salir")
                break;
        }
        sleep(1);
    }*/


    return 0;
}

/* Parametros de inicio:
 * 1) Archivo json
 * 2) Tipo de ejecucion (cliente o servidor)
 * 3) Ip
 * 4) Puerto
 *
 */

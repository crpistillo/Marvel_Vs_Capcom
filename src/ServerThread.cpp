/*
 * serverThread.cpp
 *
 *  Created on: 12 may. 2019
 *      Author: caropistillo
 */

#include "ServerThread.h"
#include <pthread.h>

ServerThread::ServerThread(TCPServer* server)
{
	this->server = server;
}

bool ServerThread::create()
{
	pthread_create(&serverThread, NULL, loop, NULL);
	return true;
}

void* ServerThread::loop(void* arg)
{

}

void ServerThread::join()
{
	pthread_join(serverThread,NULL);
}




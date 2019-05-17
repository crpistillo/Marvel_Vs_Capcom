/*
 * serverThread.h
 *
 *  Created on: 12 may. 2019
 *      Author: caropistillo
 */

#ifndef SERVERTHREAD_H_
#define SERVERTHREAD_H_

#include "ClientsThread.h"
#include "Queue/Queue.h"
#include "TCPServer.h"
#include "TCPServer.h"

class ServerThread
{
private:
	Queue<ClientsThread> clientsThreads; //cola de los threads de los clientes
	Socket* socket;
	pthread_t serverThread;
	TCPServer* server;

public:

	ServerThread(TCPServer* server);
	bool create();
	static void* loopStatic(void* arg);
	void* loop();
	//void join();


};



#endif /* SERVERTHREAD_H_ */

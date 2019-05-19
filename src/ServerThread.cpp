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
	return(pthread_create(&(this->serverThread), NULL, loopStatic, this)==0);
}
//arreglar(caro,yo)
/*
void* ServerThread::function()
{
	  // This cast will work for Linux, but in general casting pthread_id to an
	  // integral type isn't portable.
	  unsigned long id = (unsigned long)pthread_self();
	  printf("Thread %lu created to handle connection with socket %d\n", id, sockfd);
	  serve_connection(sockfd);
	  printf("Thread %lu done\n", id);
	  return 0;
}*/

void* ServerThread::loop()
{
	pthread_detach(pthread_self());
	    while (1)
	    {
	        string str = this->server->getMessage();
	        if (!str.empty()) {
	            cout << "Message:" << str << endl;
	            this->server->Send("Connection ready");
	            if(str == "salir")
	                break;
	            this->server->clean();
	        }
	    }
	    this->server->detach();

}

void* ServerThread::loopStatic(void *arg)
{
	ServerThread* obj = (ServerThread *)arg;
	obj->loop();
}


/*
void ServerThread::join()
{
	pthread_join(&serverThread,NULL);
}*/




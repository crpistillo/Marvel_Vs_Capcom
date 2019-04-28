/*
 * NetworkManager.h
 *
 *  Created on: 28 abr. 2019
 *      Author: sebastian
 */

#ifndef CLIENT_NETWORKMANAGER_H_
#define CLIENT_NETWORKMANAGER_H_

#include <iostream>
#include <string>
#include "ClientSocket.h"

class NetworkManager {
public:
	bool connectToServer(string host, int port);
	void sendMessage(string message);
	static NetworkManager* getInstance();

private:
	ClientSocket* client;
	static NetworkManager* instance;
};


#endif /* CLIENT_NETWORKMANAGER_H_ */

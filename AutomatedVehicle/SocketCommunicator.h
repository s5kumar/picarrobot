/*
 * SocketCommunicator.h
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */

#ifndef SOCKETCOMMUNICATOR_H_
#define SOCKETCOMMUNICATOR_H_
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <bits/stdc++.h>
#include "MessageProtocol.h"
#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_BUFFER_SIZE 255

using namespace std;

class SocketCommunicator {
public:

	//socket thread entry
	static void* vSocket_thread_entry(void* data);

	static SocketCommunicator* pGetSocketInstance();

	void vSetMessageProtocolHandler(MessageProtocol* messagehandler);

	static void* vMessageReadThread(void* data);

	void StartCommunication();

	void StopCommunication();

	void vDistributeMessagetoClients(int value);

private:
	SocketCommunicator();

	virtual ~SocketCommunicator();

	bool m_bIsAlreadyConnected;

	static SocketCommunicator* p_myInstance;

	pthread_t m_pSocketThreadID;

	list<pthread_t*> lMessage_processing_thread;

	list<int> lclientfd;

	int m_iServer_fd; //contains socket fd

	int m_iPortNo; //contains the port number

	static int m_iNumClients; //contains the number of clients connected

	bool m_bRunThread;

	MessageProtocol* pMsgHandler;
};

#endif /* SOCKETCOMMUNICATOR_H_ */

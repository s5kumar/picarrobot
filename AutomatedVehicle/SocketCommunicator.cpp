/*
 * SocketCommunicator.cpp
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */

#include "SocketCommunicator.h"

int SocketCommunicator::m_iNumClients = 0;
SocketCommunicator* SocketCommunicator::p_myInstance = NULL;

SocketCommunicator* SocketCommunicator::pGetSocketInstance()
{
	if(p_myInstance == NULL)
	{
		p_myInstance = new SocketCommunicator();
	}
	return p_myInstance;
}
SocketCommunicator::SocketCommunicator() {
	m_bRunThread = false;

}

SocketCommunicator::~SocketCommunicator() {
	// TODO Auto-generated destructor stub
}

void SocketCommunicator::vSetMessageProtocolHandler(MessageProtocol* msghandler)
{
	pMsgHandler = msghandler;
}

void SocketCommunicator::StartCommunication()
{
	m_bRunThread = true;
	pthread_create(&m_pSocketThreadID, NULL, &SocketCommunicator::vSocket_thread_entry, this);
}

void SocketCommunicator::StopCommunication()
{
	m_bRunThread = false;
	close(m_iServer_fd);

}
void SocketCommunicator::vDistributeMessagetoClients(int value)
{
	  for (auto itr = lclientfd.begin(); itr != lclientfd.end(); itr++) {
		  cout<<"sending  message to client" << *itr <<endl;
		  string val = to_string(value);
		  val.append(";");
	       write(*itr, val.c_str(), val.length());
	    }

}

void* SocketCommunicator::vSocket_thread_entry(void* data)
{
	cout<<"Socket thread is started"<<endl;
	SocketCommunicator* mySocketObj = (SocketCommunicator*) data;

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	int opt = 1;


	/* First call to socket() function */
	mySocketObj->m_iServer_fd = socket(AF_INET, SOCK_STREAM, 0);

   if (mySocketObj->m_iServer_fd < 0) {
	   perror("ERROR opening socket");
	  exit(EXIT_FAILURE);
   }

   // Forcefully attaching socket to the port 8080
   if (setsockopt(mySocketObj->m_iServer_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												 &opt, sizeof(opt)))
   {
	   perror("Error on setsockopt");
	   exit(EXIT_FAILURE);
   }
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons( PORT );
   /* Now bind the host address using bind() call.*/
   if (bind(mySocketObj->m_iServer_fd , (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
	   perror("ERROR on binding");
       exit(EXIT_FAILURE);
    }
   if (listen(mySocketObj->m_iServer_fd , 3) < 0)
   {
	   perror("Error on listen");
	   exit(EXIT_FAILURE);
   }
   clilen = sizeof(cli_addr);
   while(mySocketObj->m_bRunThread)
   {
		int client_fd = accept(mySocketObj->m_iServer_fd, (struct sockaddr *)&cli_addr, &clilen);

		if (client_fd < 0) {
		  perror("ERROR on accept");
		 exit(EXIT_FAILURE);
		}

		//send initial set of values to client

		if(p_myInstance->pMsgHandler!=NULL)
		{
			cout<<"sending message to new client"<<client_fd<<endl;
			string mode = to_string(p_myInstance->pMsgHandler->iGetMode());
			mode.append(";");
			string engine = to_string(p_myInstance->pMsgHandler->iGetEngineState());
			engine.append(";");
			string playmode = to_string(p_myInstance->pMsgHandler->iGetPlaybackMode());
			playmode.append(";");
			string recordstate = to_string(p_myInstance->pMsgHandler->iGetRecordState());
			recordstate.append(";");

			write(client_fd,mode.c_str(),mode.length());
			write(client_fd,engine.c_str(),engine.length());

			if(p_myInstance->pMsgHandler->iGetPlaybackMode())
				write(client_fd,playmode.c_str(),playmode.length());
			if(p_myInstance->pMsgHandler->iGetRecordState())
				write(client_fd,recordstate.c_str(),recordstate.length());

		}
		p_myInstance->lclientfd.push_back(client_fd);

		cout<<"All good...One client got connected"<<endl;
		pthread_t thread_id;

		pthread_create(&(thread_id),NULL,vMessageReadThread,&client_fd);
		mySocketObj->lMessage_processing_thread.push_back(&thread_id);
		m_iNumClients++;
   }
   return 0;
}

void* SocketCommunicator::vMessageReadThread(void* data)
{
	int *cli_sock_des = (int*)data;

	cout<<"Now a client thread started for communicating"<<endl;

	char read_buffer[MAX_BUFFER_SIZE];

	while(1)
	{
		bzero(read_buffer,MAX_BUFFER_SIZE);

		int size = read( *cli_sock_des,read_buffer,MAX_BUFFER_SIZE );

		if(size < 0 || read_buffer == NULL)
		{
			//client has probably exited
			m_iNumClients--;
			perror("Error on reading data");
			p_myInstance->lclientfd.remove(*cli_sock_des);
			exit(EXIT_FAILURE);
		}
		else if(size ==0)
		{
			//client has probably exited..exit thread
			cout<<"Now a client has exited"<<endl;
			m_iNumClients--;
			p_myInstance->lclientfd.remove(*cli_sock_des);
			break;
		}
		char* processing_message = new char[size]; //create a buffer in heap for received buffer size..shall be deleted in the client

		memcpy(processing_message,read_buffer,size);

		if(p_myInstance->pMsgHandler!=NULL)
		{
			int value = p_myInstance->pMsgHandler->vProcessMessage(processing_message);
			cout<<"process message value is"<<value<<endl;
			p_myInstance->vDistributeMessagetoClients(value);
		}
	}

	return 0;
}

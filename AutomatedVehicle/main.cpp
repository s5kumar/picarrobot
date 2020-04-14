//============================================================================
// Name        : main.cpp
// Author      : Anand Kumar
// Version     :
// Copyright   : open
// Description : Main function in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <signal.h>
#include "cGPIOHandler.h"
#include "cUltraSonicSensor.h"
#include "AutomatedVehicleEngine.h"
#include "ManualVehicleEngine.h"
#include "SocketCommunicator.h"
#include "MessageProtocol.h"
using namespace std;

/* GPIO pin numbers */
#define TRIGGER_PIN 21 //29 //21
#define ECHO_PIN 20 //26 //12
#define WHEEL_LEFT 12 //N3
#define WHEEL_RIGHT 16 //N4
#define WHEEL_FWD 19 //N1
#define WHEEL_BWD 26 //N2

bool bGlobalExit = false;

void sig_handler(int signo)
{
  if (signo == SIGUSR2)
	  bGlobalExit = true;
}


int main() {

	if (signal(SIGUSR2, sig_handler) == SIG_ERR)
	    cout<<"Can't catch SIGUSR2"<<endl;


	cUltraSonicSensor uSS (TRIGGER_PIN,ECHO_PIN);
	cGPIOHandler fwdwheel(WHEEL_FWD,false);
	cGPIOHandler reversewheel(WHEEL_BWD,false);
	cGPIOHandler leftwheel(WHEEL_LEFT,false);
	cGPIOHandler rightwheel(WHEEL_RIGHT,false);
	fwdwheel.vWritePin(0);
	reversewheel.vWritePin(0);
	leftwheel.vWritePin(0);
	rightwheel.vWritePin(0);

	AutomatedVehicleEngine autovehicle(&uSS, &fwdwheel, &reversewheel, &leftwheel, &rightwheel);
	ManualVehicleEngine manualvehicle(&uSS,&fwdwheel, &reversewheel, &leftwheel, &rightwheel);
	SocketCommunicator* psocketcom = SocketCommunicator::pGetSocketInstance();
	MessageProtocol protocol(&autovehicle, &manualvehicle);
	psocketcom->vSetMessageProtocolHandler(&protocol);
	psocketcom->StartCommunication();
	uSS.vStartSensorRead();


	//now start the camera stream
	system("raspivid -o - -t 0 -n -w 600 -h 400 -fps 12 | cvlc -vvv stream:///dev/stdin --sout '#rtp{sdp=rtsp://:8554/}' :demux=h264");

	//single threaded application....run an infinite loop
	while(!bGlobalExit)
	{
		sleep (1);
	}
	cout<<"Thread is exiting"<<endl;
	psocketcom->StopCommunication();
	uSS.vStopSensorRead();
	//pthread_join(ultrasonicsensorthread,NULL);
	return 0;
}

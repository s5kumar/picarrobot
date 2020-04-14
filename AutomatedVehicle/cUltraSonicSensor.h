/*
 * cUltraSonicSensor.h
 *
 *  Created on: 30-Mar-2020
 *      Author: anand
 */

#include "cGPIOHandler.h"
#ifndef CULTRASONICSENSOR_H_
#define CULTRASONICSENSOR_H_

class cUltraSonicSensor {
public:
	cUltraSonicSensor(int,int);
	virtual ~cUltraSonicSensor();

	void vSetupTriggerAndEcho();

	int iGetDistance();

	static void* vStartUltrasonicThread(void* uss);

	void vStartSensorRead();

	void vStopSensorRead();

	bool m_bSensorRead;

private:
	float m_distance; //holds the last distance held

	cGPIOHandler* mp_triggerpin;

	cGPIOHandler* mp_echopin;

	pthread_t m_threadid;


};

#endif /* CULTRASONICSENSOR_H_ */

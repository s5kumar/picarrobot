/*
 * cUltraSonicSensor.cpp
 *
 *  Created on: 30-Mar-2020
 *      Author: anand
 */

#include "cUltraSonicSensor.h"
#include <unistd.h>
#include <ctime>
#include <sys/time.h>
#include <iostream>
using namespace std;

void* cUltraSonicSensor::vStartUltrasonicThread(void* uss)
{
	//this thread continously update the distance
	cUltraSonicSensor* us = (cUltraSonicSensor*) uss;
	cout<<"All good...ultrasonic sensor thread started"<<endl;
	while(us->m_bSensorRead)
	{
		us->vSetupTriggerAndEcho();
		usleep(10000);

	}
	return NULL;
}

cUltraSonicSensor::cUltraSonicSensor(int triggerpin, int echopin) {
	// TODO Auto-generated constructor stub
	mp_echopin = new cGPIOHandler(echopin,true); //set read on echo pin
	mp_triggerpin = new cGPIOHandler(triggerpin,false); //set write on trigger pin
	m_distance = 0;
	mp_triggerpin->vWritePin(false);
	m_bSensorRead = false;

}

cUltraSonicSensor::~cUltraSonicSensor() {
	// TODO Auto-generated destructor stub
}

void cUltraSonicSensor::vSetupTriggerAndEcho()
{
	struct timeval tpstart;
	struct timeval tpstop;
	gettimeofday(&tpstart, NULL);
	gettimeofday(&tpstop, NULL);
	long int msstart = tpstart.tv_sec * 1000 + tpstart.tv_usec / 1000;
	long int msstop = tpstop.tv_sec * 1000 + tpstop.tv_usec / 1000;

	mp_triggerpin->vWritePin(true); //write to trigger pin

	usleep(10);

	mp_triggerpin->vWritePin(false); //basically toggle the pin

	//now wait for response from echo pin
	while(!mp_echopin->bReadValue())
	{
		gettimeofday(&tpstart, NULL);
		msstart = tpstart.tv_sec * 1000000 + tpstart.tv_usec;
	}

	while(mp_echopin->bReadValue())
	{
		gettimeofday(&tpstop, NULL);
		msstop = tpstop.tv_sec * 1000000 + tpstop.tv_usec;
	}
	long int ElapsedTime = msstop-msstart;

	float m_dist = (ElapsedTime * 34300) /2;
	if(m_dist > 0) //ignoring incorrect values
		m_distance = m_dist/1000000;

	//cout<<"My current distance is "<<m_distance<<endl;
}

int cUltraSonicSensor::iGetDistance()
{
	return (int)(m_distance);
}

void cUltraSonicSensor::vStartSensorRead()
{
	m_bSensorRead = true;
	pthread_create(&m_threadid, NULL, &cUltraSonicSensor::vStartUltrasonicThread, this);
}

void cUltraSonicSensor::vStopSensorRead()
{
	m_bSensorRead = false;
}

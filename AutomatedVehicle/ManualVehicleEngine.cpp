/*
 * ManualVehicleEngine.cpp
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */

#include "ManualVehicleEngine.h"

ManualVehicleEngine::ManualVehicleEngine(cUltraSonicSensor* sensor, cGPIOHandler* fwd, cGPIOHandler* rev, cGPIOHandler* left, cGPIOHandler* right):\
		VehicleEngine(sensor, fwd,rev, left, right) {
	// TODO Auto-generated constructor stub

}

ManualVehicleEngine::~ManualVehicleEngine() {
	// TODO Auto-generated destructor stub
}


void ManualVehicleEngine::vMoveLeft()
{
	m_determined_state=EN_TURNLEFT;
}

void ManualVehicleEngine::vMoveRight()
{
	m_determined_state=EN_TURNRIGHT;
}

void ManualVehicleEngine::vMoveFront()
{
	m_determined_state=EN_GOINGSTRAIGHT;
}

void ManualVehicleEngine::vMoveReverse()
{
	m_determined_state=EN_REVERSE;
}

void ManualVehicleEngine::vMoveReverseRight()
{
	m_determined_state=EN_REVERSERIGHT;
}

void ManualVehicleEngine::vMoveReverseLeft()
{
	m_determined_state=EN_REVERSELEFT;
}


void ManualVehicleEngine::vMoveNone()
{
	m_determined_state=EN_STOP;
}

void ManualVehicleEngine::vSteerVehicle()
{


	/*if(distance < STOP_THRESHOLD && (m_determined_state == EN_TURNLEFT || m_determined_state == EN_TURNRIGHT || m_determined_state == EN_GOINGSTRAIGHT) ) //prevent crash
	{
		m_determined_state = EN_STOP;
	}*/

	VehicleEngine::vSteerVehicle();

}

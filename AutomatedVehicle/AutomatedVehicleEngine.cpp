/*
 * AutomatedVehicleEngine.cpp
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */

#include "AutomatedVehicleEngine.h"

AutomatedVehicleEngine::AutomatedVehicleEngine(cUltraSonicSensor* sensor, cGPIOHandler* fwd, cGPIOHandler* rev, cGPIOHandler* left, cGPIOHandler* right):\
VehicleEngine(sensor, fwd,rev, left, right){
	m_Measured_distance = 0;
	m_recoverylogic = 0;
	m_tries = 0;
}

AutomatedVehicleEngine::~AutomatedVehicleEngine() {
	// TODO Auto-generated destructor stub
}

void AutomatedVehicleEngine::vSteerVehicle()
{

	if(!m_bIsActive)
		return;

	int distance=m_sensor->iGetDistance();

	if(distance == 0)
	{
		cout<<"Reading Invalid Distance from the sensor"<<endl;
		//Do nothing...invalid distance
		distance = m_Measured_distance;
	}
	//else
	{
		cout<<"The distance read out from the sensor is "<<distance<<" and the previous distance is "<<m_Measured_distance<<" and logic is "<<m_recoverylogic<<endl;
		//tolerance logic to detect if vehicle is stuck
		int diff = distance - m_Measured_distance;
		if(abs(diff) < MIN_ERROR_VALUE)
		{
			//recovery logic values
			//1 - reverse left
			//2 - right
			//3 - reverse right
			//4 - left

			m_tries ++;
			if(m_tries%MAX_TRIES_BEFORE_STUCK == 0)
			{
				m_recoverylogic ++;
			}

			if(m_recoverylogic > 4)
				m_recoverylogic = 1;
		}
		else
		{
			m_recoverylogic = 0;
			m_tries = 0;
		}
		m_Measured_distance = distance;

		if(m_recoverylogic == 1)
		{
			m_determined_state = EN_REVERSELEFT;
		}
		else if (m_recoverylogic == 2)
		{
			m_determined_state = EN_TURNRIGHT;
		}
		else if (m_recoverylogic == 3)
		{
			m_determined_state = EN_REVERSERIGHT;
		}
		else if (m_recoverylogic == 4)
		{
			m_determined_state = EN_TURNLEFT;
		}

		else if(m_Measured_distance < STOP_THRESHOLD)
		{

			m_determined_state = EN_REVERSERIGHT; //go reverse now
			//action for turning wheel to right
		}

		else if(m_Measured_distance > STOP_THRESHOLD && m_Measured_distance < TURN_THRESHOLD)
		{
			m_determined_state = EN_TURNRIGHT;
		}

		else
		{
			m_determined_state = EN_GOINGSTRAIGHT;
		}

	}
	VehicleEngine::vSteerVehicle();
	if(m_recoverylogic>0)
		usleep(200000); //give a second for the action
}

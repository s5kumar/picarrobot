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
	m_Num_tries = 0;
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
		//Do nothing...invalid distance
	}
	else
	{
		//tolerance logic to detect if vehicle is stuck
		int diff = distance - m_Measured_distance;
		if(abs(diff) < 5)
		{
			m_Num_tries++;
		}
		else
		{
			m_Num_tries = 0;
		}
		m_Measured_distance = distance;

		if(m_Num_tries > MAX_TRIES_BEFORE_STUCK && m_current_state != EN_REVERSE)
		{
			m_Num_tries = 0;
			m_determined_state = EN_REVERSE;
		}
		else if(m_Num_tries > MAX_TRIES_BEFORE_STUCK && m_current_state == EN_REVERSE)
		{
			m_determined_state = EN_GOINGSTRAIGHT;
		}
		else if(m_Measured_distance < STOP_THRESHOLD)
		{
			cout<<"5"<<endl;
			if(m_Num_tries > MAX_TRIES_BEFORE_STUCK && m_current_state != EN_REVERSE)
			{
				//looks like vehicle is stuck going forward
				m_Num_tries = 0;
			}

			m_determined_state = EN_REVERSE; //go reverse now
			sleep(1);//give it enough to turn back
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

}

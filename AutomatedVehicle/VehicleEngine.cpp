/*
 * VehicleEngine.cpp
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */

#include "VehicleEngine.h"
#include <iostream>
void* VehicleEngine::vStartEngineThread(void* vehicle_obj)
{
	VehicleEngine* myVehicle = (VehicleEngine*) vehicle_obj;

	cout<<"Engine thread is started"<<endl;
	while(myVehicle->bIsStart())
	{
		usleep(10000);
		myVehicle->vSteerVehicle();

	}
	cout<<"Engine thread is stopping"<<endl;
	return NULL;
}

VehicleEngine::VehicleEngine(cUltraSonicSensor* sensor,cGPIOHandler* fwd, cGPIOHandler* rev, cGPIOHandler* left, cGPIOHandler* right){
	m_bIsActive = false;
	m_bIsStart = false;
	m_bStartRecord = false;
	m_fwdwheel = fwd;
	m_revwheel = rev;
	m_leftwheel = left;
	m_rightwheel = right;
	m_current_state = EN_INVALID;
	m_determined_state = EN_STOP;
	m_sensor = sensor;
	m_prevtime = 0;
}

VehicleEngine::~VehicleEngine() {
	// TODO Auto-generated destructor stub
}

void VehicleEngine::vSetActive(bool value)
{
	m_bIsActive = value;
	m_record_step.clear(); //clear all previous steps
}

void VehicleEngine::vStart()
{
	if(m_bIsActive)
	{
		//only when the mode is selected, start the thread
		if(!m_bIsStart)
		{
			m_determined_state = EN_STOP;
			vSteerVehicle();
			//start the thread
			m_bIsStart = true;
			pthread_create(&m_enginethread, NULL, &VehicleEngine::vStartEngineThread, this);
		}

	}
}

void VehicleEngine::vStop()
{
	if(m_bIsActive)
	{
		if(m_bIsStart)
		{
			m_bIsStart = false; //this shall stop the thread
			pthread_join(m_enginethread, NULL); //wait for the thread to exit
		}
		cout<<"Steering vehicle to stop"<<endl;

		m_determined_state = EN_STOP;
		VehicleEngine::vSteerVehicle();
	}

}

void VehicleEngine::vMoveLeft()
{
	//do nothing...will be handled only for manual vehicle
}

void VehicleEngine::vMoveRight()
{
	//do nothing...will be handled only for manual vehicle
}

void VehicleEngine::vMoveFront()
{
	//do nothing...will be handled only for manual vehicle
}

void VehicleEngine::vMoveReverse()
{
	//do nothing...will be handled only for manual vehicle
}

void VehicleEngine::vMoveReverseRight()
{
	//do nothing...will be handled only for manual vehicle
}

void VehicleEngine::vMoveReverseLeft()
{
	//do nothing...will be handled only for manual vehicle
}


void VehicleEngine::vMoveNone()
{
	//do nothing...will be handled only for manual vehicle
}

void VehicleEngine::vStartRecord()
{
	if(m_bIsActive)
	{
		m_record_step.clear();
		struct timeval current_time;
		gettimeofday(&current_time, NULL);
		long int curtime = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
		m_prevtime = curtime;
		cout<<"time at record"<<m_prevtime<<endl;
		m_bStartRecord = true;
		record_data mydata = {0,m_current_state};
		m_record_step.push_back(mydata);
	}
}

void VehicleEngine::vStopRecord()
{
	if(m_bIsActive)
	{
		m_bStartRecord = false;
		struct timeval current_time;
		gettimeofday(&current_time, NULL);
		long int curtime = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
		long int ElapsedTime = 0;

		ElapsedTime= curtime-m_prevtime; //this was the time it was in the current state before the new change request
		m_prevtime = curtime;
		record_data mydata = {ElapsedTime,m_current_state};
		m_record_step.push_back(mydata);
	}


}

void VehicleEngine::vGoBackToStart()
{
	bool bToRestart = m_bIsStart;
	if(m_record_step.size()>0 && m_bIsActive)
	{
		//there has been a recorded entry
		if(m_bIsStart)
		{
			vStop(); //stop thread and no external control
		}
		//m_bIsActive = false;
		for (auto i = m_record_step.end(); i != m_record_step.begin(); --i)
		{
			record_data mydata = *i;
			enVehiclestate mystate = mydata.vehiclestate;

			switch(mystate)
			{
				case EN_GOINGSTRAIGHT: m_determined_state = EN_REVERSE;
					break;
				case EN_REVERSE: m_determined_state = EN_GOINGSTRAIGHT;
					break;
				case EN_TURNRIGHT: m_determined_state = EN_REVERSERIGHT;
						break;
				case EN_REVERSELEFT: m_determined_state = EN_TURNLEFT;
								break;
				case EN_TURNLEFT: m_determined_state = EN_REVERSELEFT;
						break;
				case EN_REVERSERIGHT: m_determined_state = EN_TURNRIGHT;
							break;
				case EN_STOP:
				default:
					m_determined_state = EN_STOP;

			}

			VehicleEngine::vSteerVehicle();
			usleep(mydata.step * 1000);
		}

		//m_bIsActive = true;
		if(bToRestart)
		{
			cout<<"I am restarting the thread"<<endl;
			vStart(); //restart the thread
		}

	}
}

void VehicleEngine::vPlayback()
{
	bool bToRestart=m_bIsStart;
	if(m_record_step.size()>0 && m_bIsActive)
	{
		//there has been a recorded entry
		if(m_bIsStart)
		{
			vStop(); //restart the thread
		}

		//m_bIsActive = false;

		//first go back to start
		for (auto i = m_record_step.end(); i != m_record_step.begin(); --i)
		{
			record_data mydata = *i;
			enVehiclestate mystate = mydata.vehiclestate;

			switch(mystate)
			{
				case EN_GOINGSTRAIGHT: m_determined_state = EN_REVERSE;
					break;
				case EN_REVERSE: m_determined_state = EN_GOINGSTRAIGHT;
					break;
				case EN_TURNRIGHT: m_determined_state = EN_REVERSERIGHT;
						break;
				case EN_REVERSELEFT: m_determined_state = EN_TURNLEFT;
								break;
				case EN_TURNLEFT: m_determined_state = EN_REVERSELEFT;
						break;
				case EN_REVERSERIGHT: m_determined_state = EN_TURNRIGHT;
							break;
				case EN_STOP:
				default:
					m_determined_state = EN_STOP;

			}
			VehicleEngine::vSteerVehicle();
			usleep(mydata.step * 1000);
		}


		//now playback
		for (auto i = m_record_step.end(); i != m_record_step.begin(); --i)
		{
			record_data mydata = *i;
			enVehiclestate mystate = mydata.vehiclestate;

			m_determined_state = mystate;

			VehicleEngine::vSteerVehicle();
			usleep(mydata.step * 1000);

		}

		//m_bIsActive = true;
		if(bToRestart)
		{
			vStart(); //restart the thread
		}

	}
}

bool VehicleEngine::bIsActive()
{
	return m_bIsActive;
}

bool VehicleEngine::bIsStart()
{
	return m_bIsStart;
}

void VehicleEngine::vSteerVehicle()
{


	if(m_determined_state != m_current_state)
	{
		if(m_bStartRecord)
		{
			struct timeval current_time;
			gettimeofday(&current_time, NULL);
			long int curtime = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
			long int ElapsedTime = 0;

			ElapsedTime= curtime-m_prevtime; //this was the time it was in the current state before the new change request
			m_prevtime = curtime;
			record_data mydata = {ElapsedTime,m_current_state};
			m_record_step.push_back(mydata);
			cout<<"Pushed data is "<<mydata.step<<" "<<mydata.vehiclestate<<endl;
		}
		m_current_state = m_determined_state;
		switch(m_current_state)
		{
		case EN_GOINGSTRAIGHT:
			m_fwdwheel->vWritePin(1);
			m_revwheel->vWritePin(0);
			m_leftwheel->vWritePin(0);
			m_rightwheel->vWritePin(0);
			break;
		case EN_TURNRIGHT:
			m_fwdwheel->vWritePin(1);
			m_revwheel->vWritePin(0);
			m_leftwheel->vWritePin(0);
			m_rightwheel->vWritePin(1);
			//sleep(2);
			break;
		case EN_TURNLEFT:
			m_fwdwheel->vWritePin(1);
			m_revwheel->vWritePin(0);
			m_leftwheel->vWritePin(1);
			m_rightwheel->vWritePin(0);
		//	sleep(2);
			break;
		case EN_REVERSE:
			m_fwdwheel->vWritePin(0);
			m_revwheel->vWritePin(1);
			m_leftwheel->vWritePin(0);
			m_rightwheel->vWritePin(0);
			//sleep(2);
			break;
		case EN_REVERSERIGHT:
			m_fwdwheel->vWritePin(0);
			m_revwheel->vWritePin(1);
			m_leftwheel->vWritePin(0);
			m_rightwheel->vWritePin(1);
				//sleep(2);
			break;
		case EN_REVERSELEFT:
			m_fwdwheel->vWritePin(0);
			m_revwheel->vWritePin(1);
			m_leftwheel->vWritePin(1);
			m_rightwheel->vWritePin(0);
			//sleep(2);
			break;
		case EN_STOP:
		default:
			m_fwdwheel->vWritePin(0);
			m_revwheel->vWritePin(0);
			m_leftwheel->vWritePin(0);
			m_rightwheel->vWritePin(0);
			break;
		}

	}
}

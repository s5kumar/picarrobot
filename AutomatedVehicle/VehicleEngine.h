/*
 * VehicleEngine.h
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */

#ifndef VEHICLEENGINE_H_
#define VEHICLEENGINE_H_
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <sys/time.h>


#include "cGPIOHandler.h"
#include "cUltraSonicSensor.h"

#define TURN_THRESHOLD 60
#define STOP_THRESHOLD 30
using namespace std;
class VehicleEngine {
public:
	VehicleEngine(cUltraSonicSensor* sensor, cGPIOHandler* fwd, cGPIOHandler* rev, cGPIOHandler* left, cGPIOHandler* right);
	virtual ~VehicleEngine();

	enum enVehiclestate
	{
		EN_STOP = 0,
		EN_GOINGSTRAIGHT,
		EN_TURNRIGHT,
		EN_TURNLEFT,
		EN_REVERSE,
		EN_REVERSELEFT,
		EN_REVERSERIGHT,
		EN_INVALID
	};

	void vSetActive(bool value);

	virtual void vStart();

	virtual void vStop();

	virtual void vMoveLeft();

	virtual void vMoveRight();

	virtual void vMoveFront();

	virtual void vMoveReverse();

	virtual void vMoveReverseLeft();

	virtual void vMoveReverseRight();

	virtual void vMoveNone();

	virtual void vStartRecord();

	virtual void vStopRecord();

	virtual void vGoBackToStart();

	virtual void vPlayback();

	virtual void vSteerVehicle(); //this shall be different in different vehicles

	bool bIsActive();

	bool bIsStart();

	//thread entry function
	static void* vStartEngineThread(void* vehicle_obj);

protected:

	struct record_data
	{
		long int step;
		enVehiclestate vehiclestate;
	};
	bool m_bIsActive;

	pthread_t m_enginethread;

	bool m_bIsStart;

	bool m_bStartRecord;

	cGPIOHandler* m_leftwheel;

	cGPIOHandler* m_rightwheel;

	cGPIOHandler* m_fwdwheel;

	cGPIOHandler* m_revwheel;

	cUltraSonicSensor* m_sensor;

	enVehiclestate m_current_state;

	enVehiclestate m_determined_state;

	vector<record_data> m_record_step;

	long int m_prevtime;

};

#endif /* VEHICLEENGINE_H_ */

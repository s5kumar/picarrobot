/*
 * AutomatedVehicleEngine.h
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */

#include "VehicleEngine.h"
#ifndef AUTOMATEDVEHICLEENGINE_H_
#define AUTOMATEDVEHICLEENGINE_H_


#define MAX_TRIES_BEFORE_STUCK 3

class AutomatedVehicleEngine: public VehicleEngine {
public:
	AutomatedVehicleEngine(cUltraSonicSensor* sensor, cGPIOHandler* fwd, cGPIOHandler* rev, cGPIOHandler* left, cGPIOHandler* right);
	virtual ~AutomatedVehicleEngine();

	void vSteerVehicle();

private:
	int m_Measured_distance;

	int m_Num_tries;
};

#endif /* AUTOMATEDVEHICLEENGINE_H_ */

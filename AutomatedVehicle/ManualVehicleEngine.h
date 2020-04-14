/*
 * ManualVehicleEngine.h
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */
#include "VehicleEngine.h"

#ifndef MANUALVEHICLEENGINE_H_
#define MANUALVEHICLEENGINE_H_

class ManualVehicleEngine : public VehicleEngine {
public:
	ManualVehicleEngine(cUltraSonicSensor* sensor, cGPIOHandler* fwd, cGPIOHandler* rev, cGPIOHandler* left, cGPIOHandler* right);
	virtual ~ManualVehicleEngine();

	void vMoveLeft();

	void vMoveRight();

	void vMoveFront();

	void vMoveReverse();

	void vMoveReverseRight();

	void vMoveReverseLeft();

	void vMoveNone();

	void vSteerVehicle();

};

#endif /* MANUALVEHICLEENGINE_H_ */

/*
 * MessageProtocol.h
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */
#include "VehicleEngine.h"
#include <string>

#ifndef MESSAGEPROTOCOL_H_
#define MESSAGEPROTOCOL_H_

class MessageProtocol {
public:
	MessageProtocol(VehicleEngine* automated, VehicleEngine* manual);
	virtual ~MessageProtocol();

	enum msg_ids
	{
		EN_ACTIVATE_MANUAL = 0,
		EN_ACTIVATE_AUTO = 1,
		EN_STARTENGINE = 2,
		EN_STOPENGINE = 3,
		EN_MOVEFRONT = 4,
		EN_MOVELEFT = 5,
		EN_MOVERIGHT = 6,
		EN_MOVEREVERSE = 7,
		EN_MOVEREVERSERIGHT = 8,
		EN_MOVEREVERSELEFT = 9,
		EN_MOVENONE = 10,
		EN_STARTRECORD = 11,
		EN_STOPRECORD = 12,
		EN_GOBACKTOSTART = 13,
		EN_PLAYBACK = 14
	};
	enum return_code
	{
		EN_SUCCESS = 0,
		EN_PLAYMODE_ACTIVE = 1,
		EN_PLAYMODE_ERROR = 2,
		EN_RECORDMODE_ACTIVE = 3,
		EN_STARTENGINE_ACTIVE = 4,
		EN_STOPENGINE_ACTIVE = 5,
		EN_AUTOMATICMODE_ACTIVE = 6,
		EN_MANUALMODE_ACTIVE = 7,
		EN_FAILURE = 8,
		EN_RECORDMODE_INACTIVE = 9,
		EN_PLAYBACKMODE_INACTIVE = 10
	};

	int vProcessMessage(char* msg_buffer);

	int iGetMode();
	int iGetRecordState();
	int iGetEngineState();
	int iGetPlaybackMode();
private:

	VehicleEngine* mp_automated_engine;
	VehicleEngine* mp_manual_engine;

	bool isAutomated;
	bool isRecordActive;
	bool isEngineStarted;
	bool isPlayMode;
};

#endif /* MESSAGEPROTOCOL_H_ */

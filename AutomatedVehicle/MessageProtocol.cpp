/*
 * MessageProtocol.cpp
 *
 *  Created on: 05-Apr-2020
 *      Author: anand
 */

#include "MessageProtocol.h"
#include "SocketCommunicator.h"

MessageProtocol::MessageProtocol(VehicleEngine* automated, VehicleEngine* manual) {
	// TODO Auto-generated constructor stub
	mp_automated_engine = automated;
	mp_manual_engine = manual;
	isAutomated = false;
	isPlayMode = false;
	isRecordActive = false;
	isEngineStarted = false;

	mp_manual_engine->vSetActive(true);

}

MessageProtocol::~MessageProtocol() {
	// TODO Auto-generated destructor stub
}

int MessageProtocol::vProcessMessage(char* msg_buffer)
{
	int msg_id = atoi(&msg_buffer[0]); //first message indicates the msg_id

	cout<<"Received message from client"<<msg_id<<endl;
	switch(msg_id){
		case EN_ACTIVATE_AUTO: //automated or vehicle
			if(!isAutomated && !isPlayMode)
			{
				//was in manual mode
				if(isRecordActive) //manual mode and record was active
				{
					mp_manual_engine->vStopRecord();
					SocketCommunicator::pGetSocketInstance()->vDistributeMessagetoClients(EN_RECORDMODE_INACTIVE);
					isRecordActive = false;
				}
				mp_manual_engine->vStop();
				SocketCommunicator::pGetSocketInstance()->vDistributeMessagetoClients(EN_STOPENGINE_ACTIVE);
				mp_automated_engine->vSetActive(true);
				mp_manual_engine->vSetActive(false);
				isAutomated = true;
				return EN_AUTOMATICMODE_ACTIVE;
			}
			else if (!isPlayMode)
			{
				return EN_PLAYMODE_ERROR;
			}
			break;
		case EN_ACTIVATE_MANUAL: //automated or vehicle
			if(isAutomated && !isPlayMode)
			{
				if(isRecordActive)
				{
					mp_automated_engine->vStopRecord();
					SocketCommunicator::pGetSocketInstance()->vDistributeMessagetoClients(EN_RECORDMODE_INACTIVE);
					isRecordActive=false;
				}
				mp_automated_engine->vStop();
				SocketCommunicator::pGetSocketInstance()->vDistributeMessagetoClients(EN_STOPENGINE_ACTIVE);
				mp_automated_engine->vSetActive(false);
				mp_manual_engine->vSetActive(true);
				isAutomated=false;
				return EN_MANUALMODE_ACTIVE;
			}
			else if (!isPlayMode)
			{
				return EN_PLAYMODE_ERROR;
			}
			break;
		case EN_STARTENGINE:
			if(isAutomated)
				mp_automated_engine->vStart();
			else
				mp_manual_engine->vStart();

			isEngineStarted = true;
			return EN_STARTENGINE_ACTIVE;
			break;
		case EN_STOPENGINE:
			if(isAutomated)
				mp_automated_engine->vStop();
			else
				mp_manual_engine->vStop();
			isEngineStarted = false;
			return EN_STOPENGINE_ACTIVE;
			break;
		case EN_MOVELEFT:
			if(isAutomated)
				return EN_FAILURE;
			mp_manual_engine->vMoveLeft();
			break;
		case EN_MOVERIGHT:
			if(isAutomated)
				return EN_FAILURE;
			mp_manual_engine->vMoveRight();
			break;
		case EN_MOVEFRONT:
			if(isAutomated)
				return EN_FAILURE;
			mp_manual_engine->vMoveFront();
			break;
		case EN_MOVEREVERSE:
			if(isAutomated)
				return EN_FAILURE;
			mp_manual_engine->vMoveReverse();
			break;
		case EN_MOVEREVERSERIGHT:
			if(isAutomated)
				return EN_FAILURE;

			mp_manual_engine->vMoveReverseRight();
				break;
		case EN_MOVEREVERSELEFT:
			if(isAutomated)
				return EN_FAILURE;

			mp_manual_engine->vMoveReverseLeft();
				break;
		case EN_MOVENONE:
			if(isAutomated)
				return EN_FAILURE;
			mp_manual_engine->vMoveNone();
			break;
		case EN_STARTRECORD:
			if(isAutomated)
				mp_automated_engine->vStartRecord();
			else
				mp_manual_engine->vStartRecord();
			isRecordActive = true;
			return EN_RECORDMODE_ACTIVE;
			break;
		case EN_STOPRECORD:
			if(isAutomated)
				mp_automated_engine->vStopRecord();
			else
				mp_manual_engine->vStopRecord();
			isRecordActive = false;
			return EN_RECORDMODE_INACTIVE;
			break;
		case EN_GOBACKTOSTART:
			isPlayMode = true;
			if(isAutomated)
				mp_automated_engine->vGoBackToStart();
			else
				mp_manual_engine->vGoBackToStart();
			isPlayMode = false;
			return EN_PLAYBACKMODE_INACTIVE;
			break;
		case EN_PLAYBACK:
			isPlayMode = true;
			if(isAutomated)
				mp_automated_engine->vPlayback();
			else
				mp_manual_engine->vPlayback();
			isPlayMode = false;
			return EN_PLAYBACKMODE_INACTIVE;
			break;
		default:
			break;

	}
	return EN_SUCCESS;
}

int MessageProtocol::iGetEngineState()
{
	if(isEngineStarted)
		return EN_STARTENGINE_ACTIVE;
	else
		return EN_STOPENGINE_ACTIVE;
}
int MessageProtocol::iGetMode()
{
	if(isAutomated)
		return EN_AUTOMATICMODE_ACTIVE;
	else
		return EN_MANUALMODE_ACTIVE;
}
int MessageProtocol::iGetPlaybackMode()
{
	if(isPlayMode)
		return EN_PLAYMODE_ACTIVE;
	else
		return 0;
}
int MessageProtocol::iGetRecordState()
{
	if(isRecordActive)
		return EN_RECORDMODE_ACTIVE;
	else
		return 0;
}

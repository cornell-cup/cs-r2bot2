#ifndef _R2BOT_SOUND_CONTROLLER
#define _R2BOT_SOUND_CONTROLLER

#include "Global.h"
#include "Controller.h"

class SoundController : public Controller {
public:
	SoundController();
	virtual ~SoundController();
	
	virtual bool ping();

	virtual void sendData(ControllerData& data);
};

#endif
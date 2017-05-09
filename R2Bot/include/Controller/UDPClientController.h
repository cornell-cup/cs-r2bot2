#ifndef _R2BOT_UDPCLIENT_SENSOR
#define _R2BOT_UDPCLIENT_SENSOR

#include "Global.h"
#include "Controller.h"
#include "R2Protocol.hpp"
#include "UDPSocketClient.h"

class UDPClientController : public Controller {
protected:
	string destination;
	ptr<UDPSocketClient> client;
public:
	UDPClientController(string destination, string host, int port);
	virtual ~UDPClientController();

	virtual bool ping();
	virtual void sendData(ControllerData & data);
};

#endif
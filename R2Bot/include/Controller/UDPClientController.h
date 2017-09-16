#ifndef _R2BOT_UDPCLIENT_SENSOR
#define _R2BOT_UDPCLIENT_SENSOR

#include "Global.h"
#include "Controller.h"
#include "R2Protocol.hpp"
#include "UDPSocketClient.h"

#include <string>

class UDPClientController : public Controller {
protected:
	std::string destination;
	ptr<UDPSocketClient> client;
public:
	UDPClientController(std::string destination, std::string host, int port);
	virtual ~UDPClientController();

	virtual bool ping();
	virtual void sendData(ControllerData & data);
};

#endif
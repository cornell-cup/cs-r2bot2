#ifndef _R2BOT_UDPCLIENT_SENSOR
#define _R2BOT_UDPCLIENT_SENSOR

#include "Global.h"
#include "Controller.h"
#include "Util/UDPSocketClient.h"

class UDPClientController : public Controller {
protected:
	ptr<UDPSocketClient> client;
public:
	UDPClientController(string host, int port);
	~UDPClientController();

	bool ping();

	void sendData(std::string data);
};


#endif
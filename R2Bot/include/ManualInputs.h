#pragma once
#include <string>
#include <queue>
#include "UDPSocketServer.h"
#include "Sensor.h"
class ManualInputs
{
protected:
	/** A UDPSocketServer object to listen for commands */
	UDPSocketServer *commandListener;
	/** Queue storing the manual commands*/
	std::queue<std::string> *commands;
public:
	/** Given a hostname and port, constructs a UDPSocketServer object to listen for commands */
	ManualInputs(std::string host, int port);
	/** Receive job commands to place on the queue in a thread */
	void receiveManualInputs();
	/** Returns the queue of the manual commands */
	std::queue<std::string>* getCommands();

};
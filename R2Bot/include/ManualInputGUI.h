#include <string>
#include <queue>
#include "UDPSocketServer.h"
#include "Sensor.h"
class ManualInputsGUI
{
protected:
	/** Queue storing the manual commands*/
	std::queue<std::string> *moves;

public:
	float leftMotorSpeed;
	float rightMotorSpeed;

	/** Given a hostname and port, constructs a UDPSocketServer object to listen for commands */
	ManualInputsGUI();

	/**Calculate the speed of the motors from the coordinates of the GUI*/
	void calcMotorSpeed(float lStickX, float lStickY);

	/** Receive job commands to place on the queue in a thread */
	void receiveData();
	/** Returns the queue of the manual commands */
	std::queue<std::string>* getMoves();

};


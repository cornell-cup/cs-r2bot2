#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include <Windows.h>
#include <Xinput.h>
#include <iostream>

class XBoxController
{
public:

	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;
	float leftMotorSpeed;
	float rightMotorSpeed;

	// Constructors and Destructors
	XBoxController();
	XBoxController(float, float);
	~XBoxController();

	//Accessor Functions
	float getDeadzoneX() const;
	float getDeadzoneY() const;
	XINPUT_GAMEPAD *getState();
	bool CheckConnection();
	bool Refresh();

	//Mutator Functions
	void setDeadzoneX(float);
	void setDeadzoneY(float);
	void setMaxSpeed(float);
	void calcSimpleMotorSpeed(float, float);
	void calcTankDriveMotorVoltage(float, float);

private:
	int controllerId;
	XINPUT_STATE state;
	float deadzoneX;
	float deadzoneY;
	float maxSpeed;



};

#endif
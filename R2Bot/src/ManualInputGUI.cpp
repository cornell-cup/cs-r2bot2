#include "ManualInputGUI.h"

ManualInputsGUI::ManualInputsGUI()
{
	moves = new std::queue<string>();
}

void ManualInputsGUI::calcMotorSpeed(float lStickX, float lStickY)
{
	leftMotorSpeed = 0;
	rightMotorSpeed = 0;
	if (lStickX > -0.15 && lStickX < 0.15 && (lStickY > 0.15 || lStickY < -0.15)) {
		leftMotorSpeed = lStickY * 447;
		rightMotorSpeed = lStickY * -447;
	}
	if (lStickY > -0.15 && lStickY < 0.15 && (lStickX > 0.15 || lStickX < -0.15)) {
		leftMotorSpeed = lStickX * 447;
		rightMotorSpeed = lStickX * -447;
	}
}

void ManualInputsGUI::receiveData()
{

}

std::queue<std::string>* ManualInputsGUI::getMoves()
{
	return moves;
}

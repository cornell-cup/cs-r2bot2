#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include "XBoxController.h"
#include <XInput.h>
#include <iostream>


using std::cout;
using std::endl;

XBoxController::XBoxController() {
	deadzoneX = 0.15f;
	deadzoneY = 0.15f;
}


XBoxController::XBoxController(float dx, float dy) {
	deadzoneX = dx;
	deadzoneY = dy;
}

XBoxController::~XBoxController() {}

float XBoxController::getDeadzoneX() const {
	return deadzoneX;
}
float XBoxController::getDeadzoneY() const {
	return deadzoneY;
}

XINPUT_GAMEPAD *XBoxController::getState() {
	return &state.Gamepad;
}

bool XBoxController::CheckConnection()
{
	int cId = -1;

	for (int i = 0; i < XUSER_MAX_COUNT && cId == -1; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			cId = i;
	}

	controllerId = cId;

	return cId != -1;
}

bool XBoxController::Refresh()
{
	if (controllerId == -1)
		CheckConnection();

	if (controllerId != -1)
	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(controllerId, &state) != ERROR_SUCCESS)
		{
			controllerId = -1;
			return false;
		}

		//Normalizes X and Y values to a value between -1 and 1
		float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

		/*Sets the left stick X and Y values to either 0 if they are within the bounds of the deadzone
		or to abs(normLx) - deadzoneX (while preserving the sign)*/
		leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
		leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));

		/*Scales up the value of leftStickX leftStick Y to a value between 0 and 1*/
		if (deadzoneX > 0) leftStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) leftStickY *= 1 / (1 - deadzoneY);

		//Normalizes X and Y values to a value between -1 and 1
		float normRX = fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767);

		/*Sets the right stick X and Y values to either 0 if they are within the bounds of the deadzone
		or to abs(normLx) - deadzoneX (while preserving the sign)*/
		rightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
		rightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));

		/*Scales up the value of rightStickX rightStickY to a value between 0 and 1*/
		if (deadzoneX > 0) rightStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) rightStickY *= 1 / (1 - deadzoneY);


		return true;
	}
	return false;
}


void XBoxController::setDeadzoneX(float dz) {
	deadzoneX = dz;
}

void XBoxController::setDeadzoneY(float dz) {
	deadzoneY = dz;
}

void XBoxController::calcSimpleMotorSpeed(float lStickX, float lStickY) {
	leftMotorSpeed = 0;
	rightMotorSpeed = 0;
	if (lStickX > -0.15 && lStickX < 0.15 && (lStickY > 0.15 || lStickY < -0.15)) {
		leftMotorSpeed = lStickY * 80;
		rightMotorSpeed = lStickY * 80;
	}
	if (lStickY > -0.15 && lStickY < 0.15 && (lStickX > 0.15 || lStickX < -0.15)) {
		leftMotorSpeed = lStickX * 40;
		rightMotorSpeed = lStickX * -40;
	}


}

void XBoxController::calcTankDriveMotorVoltage(float rStickX, float rStickY) {
	float invX = -1.0 *  rStickX;
	float v = (1 - abs(invX)) * (rStickY)+rStickY;
	float w = (1 - abs(rStickY)) * (invX)+invX;
	float r = (v + w) / 2;
	float l = (v - w) / 2;
	leftMotorSpeed = l * 80;
	rightMotorSpeed = r * 80;


}


void XBoxController::setMaxSpeed(float voltage) {
	if (abs(voltage) > 80.0) {
		throw std::invalid_argument("received voltage magnitude over 80V");
	}
	else {
		maxSpeed = voltage;
	}
}


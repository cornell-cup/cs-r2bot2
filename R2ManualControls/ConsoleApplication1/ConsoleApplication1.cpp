// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <Windows.h>
#include <Xinput.h>
#include "XBoxController.h"

using std::cout;
using std::endl;

int main() {
	XBoxController xc(0.15f, 0.15f);
	bool wasConnected = true;

	while (true) {
		Sleep(300);

		if (!xc.Refresh()) {
			if (wasConnected) {
				wasConnected = false;
				std::cout << "Connect a controller please";
			}
		}
		else
			if (!wasConnected) {
				wasConnected = true;
				std::cout << "Controller is connected" << endl;
			}
		std::cout << "Left thumb stick: (" << xc.leftStickX << ", " << xc.leftStickY <<
			")    Right thumb stick : (" << xc.rightStickX << ", " << xc.rightStickY << ")" <<
			endl;
		xc.calcVoltage(xc.leftStickX, xc.leftStickY);
		std::cout << "Left Voltage: (" << xc.leftVoltage << ",    " << "<< Right Voltage: ( " << xc.rightVoltage << endl;
	}
}

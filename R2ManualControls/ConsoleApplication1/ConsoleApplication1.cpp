// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <Xinput.h>
#include "XBoxController.h"
#include <stdio.h>
#include <string>
#include <chrono>

#include "UDPSocketClient.h"



using std::cout;
using std::endl;

// Exit
volatile int quit = 0;

#if _WIN32
int interruptHandler(unsigned int type) {
	switch (type) {
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		quit = 1;
		return TRUE;
	default:
		return FALSE;
	}
}
#else
void interruptHandler(int type) {
	quit = 1;
}
#endif

/**
* Initialize WinSock
*/
void initWSA() {
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
}

// This main function is for testing all the socket and serial port classes


int main(int argc, char** argv) {

	/**if (argc < 2) {
	printf("Usage: %s <command> [additional options]\n", argv[0]);
	printf("        Commands:\n");
	printf("            serial    <COM> <baudrate>\n");
	printf("            udpserver <address> <port>\n");
	printf("            tcpserver <address> <port>\n");
	printf("            udpclient <address> <port>\n");
	printf("            tcpclient <address> <port>\n");
	return 1;
	}*/

	// Set interrupt handler
#if _WIN32
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)interruptHandler, TRUE);
#else
	signal(SIGINT, interruptHandler);
#endif
	XBoxController xc(0.15f, 0.15f);
	bool wasConnected = true;
	initWSA();

	UDPSocketClient conn("192.168.4.170", 9020);
	if (conn.isConnected()) {
		std::chrono::time_point<std::chrono::system_clock> start, end;

		start = std::chrono::system_clock::now();
		char * buffer = new char[4096];
		memset(buffer, 0, 4096);
		while (true) {


			Sleep(1000);

			if (!xc.Refresh()) {
				if (wasConnected) {
					wasConnected = false;
					std::cout << "Connect a controller please";
				}
			}
			else {
				if (!wasConnected) {
					wasConnected = true;
					std::cout << "Controller is connected" << endl;
				}
				//string send = "Left thumb stick: (" + xc.leftStickX + ", " + std::to_string(xc.leftStickY) +
				")    Right thumb stick : (" + std::to_string(xc.rightStickX) + ", " + std::to_string(xc.rightStickY) + ")" + "\n";
				xc.calcTankDriveMotorVoltage(xc.leftStickX, xc.leftStickY);
				std::string a = "XBOX Kinect " + std::to_string(xc.leftMotorVoltage) + " " + std::to_string(xc.rightMotorVoltage);
				//std::string send2 = "Left Voltage: (" + std::to_string(xc.leftVoltage) + ",    " + " Right Voltage: ( " + std::to_string(xc.rightVoltage) + "\n";
				//std::string send = std::to_string(5);
				//std::string send2 = std::to_string(6);
				cout << a.c_str();

				conn.write(a.c_str(), 100);
			}

		}

	}
	else {
		while (true) { std::cout << "o noes"; }
	}





	/**while (true) {
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
	} */
}

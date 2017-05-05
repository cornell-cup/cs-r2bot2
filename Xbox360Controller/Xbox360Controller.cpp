
#include "R2Protocol.hpp"
#include "UDPSocketClient.h"
#include "Data/GamepadData.h"

#include <Windows.h>
#include <Xinput.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

/**
* Initialize WinSock
*/
#ifdef _WIN32
void initWSA() {
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
}
#endif

void checkConnection(int& controllerId) {
	for (int i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++) {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS) {
			controllerId = i;
		}
	}
}

int main(int argc, char** argv) {
	if (argc < 3) {
		printf("Usage: %s <host> <port>\n", argv[0]);
		return 0;
	}

#ifdef _WIN32
	initWSA();
#endif

	std::string host(argv[1]);
	int port = atoi(argv[2]);
	UDPSocketClient client(host, port);

	XINPUT_STATE state;

	int controllerId = -1;
	while (1) {
		// Find a controller if not found
		if (controllerId == -1) {
			checkConnection(controllerId);
		}

		// Get the state of the current controller
		if (controllerId == -1) {
			continue;
		}
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(controllerId, &state) != ERROR_SUCCESS)
		{
			controllerId = -1;
			continue;
		}

		// Get normalized X and Y values to [-1, 1]
		float normLX = (float) state.Gamepad.sThumbLX / (float) (1 << 14);
		float normLY = (float) state.Gamepad.sThumbLY / (float) (1 << 14);
		printf("%f %f\n", normLX, normLY);

		// Send these values over UDP
		if (client.isConnected()) {
			GamepadData g = { normLX, normLY };
			uint8_t * buffer = (uint8_t *)malloc(sizeof(GamepadData));
			memcpy(buffer, &g, sizeof(GamepadData));
			std::vector<uint8_t> output;
			R2Protocol::Packet params;
			params.source = "GAMEPAD";
			params.destination = "PI";
			params.id = "";
			params.data = std::vector<uint8_t>(buffer, buffer + sizeof(GamepadData));
			R2Protocol::encode(params, output);
			client.write((char *) output.data(), (unsigned int) output.size());
			free(buffer);
		}

		// Wait
		Sleep(50);
	}
    return 0;
}

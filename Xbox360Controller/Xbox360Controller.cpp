
#include "R2Protocol.hpp"
#include "UDPSocketClient.h"

#include <Windows.h>
#include <Xinput.h>

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
	char buffer[64];

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
		float normLX = (float) state.Gamepad.sThumbLX / (float) (1 << 13);
		float normLY = (float) state.Gamepad.sThumbLY / (float) (1 << 13);

		// Send these values over UDP
		sprintf_s(buffer, 64, "%f %f", normLX, normLY);
		if (client.isConnected()) {
			std::vector<uint8_t> output;
			R2Protocol::Packet params;
			params.source = "gamepad";
			params.destination = "nuc";
			params.id = "0";
			params.data = std::vector<uint8_t>(buffer, buffer + strlen(buffer));
			R2Protocol::encode(params, output);
			client.write((char *) output.data(), output.size());
		}

		// Wait
		Sleep(16);
	}
    return 0;
}

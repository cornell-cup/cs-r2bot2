
#include "R2Protocol.hpp"
#include "UDPSocketClient.h"
#include "Data/GamepadData.h"

#include <stdio.h>
#include <gamepad.h>

int main(int argc, char** argv) {
	if (argc < 3) {
		printf("Usage: %s <host> <port>\n", argv[0]);
		return 0;
	}

	std::string host(argv[1]);
	int port = atoi(argv[2]);
	UDPSocketClient client(host, port);

    GamepadInit();

	while (1) {
        GAMEPAD_DEVICE dev = GAMEPAD_0;

        GamepadUpdate();

        float lx, ly;

        if (!GamepadIsConnected(dev)) {
            printf("Gamepad not connected\n");
            continue;
        }

        GamepadStickNormXY(dev, STICK_LEFT, &lx, &ly);
        bool leftButton = GamepadTriggerLength(dev, TRIGGER_LEFT) > 0.5;
        bool rightButton = GamepadTriggerLength(dev, TRIGGER_RIGHT) > 0.5;

        float normLX = lx;
        float normLY = ly;

		printf("%f %f %d %d\n", normLX, normLY, leftButton, rightButton);

		// Send these values over UDP
		if (client.isConnected()) {
			if (normLX != 0 && normLY != 0) {
				ptr<GamepadData> g = std::make_shared<GamepadData>();
				g->x = normLX;
				g->y = normLY;
				g->lb = leftButton;
				g->rb = rightButton;
				uint8_t * buffer = (uint8_t *)malloc(sizeof(GamepadData));
				memcpy(buffer, g.get(), sizeof(GamepadData));
				std::vector<uint8_t> output;
				R2Protocol::Packet params;
				params.source = "GAMEPAD";
				params.destination = "PI";
				params.id = "";
				params.data = std::vector<uint8_t>(buffer, buffer + sizeof(GamepadData));
				R2Protocol::encode(params, output);
				client.write((char *)output.data(), (unsigned int)output.size());
				free(buffer);
			}
		}

		// Wait
		Sleep(50);
	}
    return 0;
}

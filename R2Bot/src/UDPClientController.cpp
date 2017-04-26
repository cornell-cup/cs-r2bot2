#include "Controller/UDPClientController.h"
#include "Data/ForwardData.h"

UDPClientController::UDPClientController(string host, int port) : Controller("UDP Client"), client(std::make_shared<UDPSocketClient>(host, port)) {
}

UDPClientController::~UDPClientController() {
}

bool UDPClientController::ping() {
	return client->isConnected() == 1;
}

void UDPClientController::sendData(ControllerData & data) {
	if (client->isConnected()) {
		auto result = data.find("FORWARD");
		if (result != data.end()) {
			ptr<ForwardData> c = std::static_pointer_cast<ForwardData>(result->second);
			for (auto s : c->data) {
				std::vector<uint8_t> output;
				R2Protocol::encode(s.second, output);
				client->write((const char *)output.data(), (unsigned int)output.size());
			}
		}
	}
}
#include "stdafx.h"
#include "UDPSocketClient.h"

UDPSocketClient::UDPSocketClient(std::string inAddress, int inPort) :
	address(inAddress), port(inPort), connected(0) {
	if ((socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
		printf("UDPSocketClient: Could not create socket.\n");
		return;
	}

	addr.sin_family = AF_INET;
	// TODO Support IPv6, use InetPton (inet_addr is deprecated)
	addr.sin_addr.s_addr = inet_addr(address.c_str());
	addr.sin_port = htons(port);

	if (connect(socketId, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("UDPSocketClient: Error binding to the socket address.\n");
		return;
	}

	connected = true;
}

UDPSocketClient::~UDPSocketClient() {
	if (connected) {
		closesocket(socketId);
	}

	connected = false;
}

int UDPSocketClient::isConnected() {
	return connected;
}

int UDPSocketClient::read(char * outBuffer, unsigned int buffer_len) {
	unsigned int len;

	len = recv(socketId, outBuffer, buffer_len, NULL);

	return len;
}

int UDPSocketClient::write(const char * buffer, unsigned int len) {
	if (send(socketId, buffer, len, NULL) < 0) {
		return 0;
	}
	// TODO Handle error codes and reconnection

	return 1;
}

void UDPSocketClient::close() {
	if (connected) {
		closesocket(socketId);
	}

	connected = false;
}
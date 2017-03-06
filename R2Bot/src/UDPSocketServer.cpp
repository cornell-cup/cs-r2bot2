#include "Util/UDPSocketServer.h"
#include <cstring>
#include <queue>

UDPSocketServer::UDPSocketServer(std::string inAddress, int inPort) :
	address(inAddress), port(inPort), listening(0), closeMessage(0) {
	if ((socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
		printf("UDPSocketServer: Could not create socket.\n");
		return;
	}

	addr.sin_family = AF_INET;
	// TODO Support IPv6, use InetPton (inet_addr is deprecated)
	addr.sin_addr.s_addr = inet_addr(address.c_str());
	addr.sin_port = htons(port);

	if (bind(socketId, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("UDPSocketServer: Error binding to the socket address.\n");
		return;
	}
	closeMessage = 0;
}

UDPSocketServer::~UDPSocketServer() {
	close();
}

int UDPSocketServer::isListening() {
	return listening;
}

void UDPSocketServer::server(std::function<void(char *, unsigned int)> handler) {
	std::thread(&UDPSocketServer::handle, this, handler).detach();
	listening = 1;
}

void UDPSocketServer::handle(std::function<void(char *, unsigned int)> handler) {
	char * buffer = new char[DEFAULT_BUFFER_SIZE];
	unsigned int buffer_len = DEFAULT_BUFFER_SIZE;
	unsigned int len = 0;

	std::memset(buffer, 0, buffer_len);
	while (!closeMessage) {
		len = recv(socketId, buffer, buffer_len, 0);
		if (len > 0) {
			handler(buffer, len);
			std::memset(buffer, 0, buffer_len);
		}
	}

	closeMessage = 0;
	delete buffer;
}

void UDPSocketServer::close() {
#ifdef _WIN32
	closesocket(socketId);
#else
	::close(socketId);
#endif
	closeMessage = 1;
}
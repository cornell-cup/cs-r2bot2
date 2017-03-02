#pragma once

#include <stdio.h>
#include <string>
#include <functional>
#include <thread>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFFER_SIZE 4096
#define SOCKET_READ_TIMEOUT_SEC 1

/**
 * UDP socket client
 *
 * Refer to the following link for Microsoft's Server C++ code:
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms737593(v=vs.85).aspx
 */
class UDPSocketClient {
private:
	/**
	 * The address to connect to
	 */
	std::string address;

	/**
	 * The port to connect to
	 */
	int port;

	/**
	 * Associated socket id
	 */
	int socketId;

	/**
	 * Bound socket address
	 */
	SOCKADDR_IN addr;

	/**
	 * Whether or not the server is listening
	 */
	int connected;

public:
	/**
	 * Initialize a new UDP socket server.
	 *
	 * @param address	The address to bind to
	 * @param port		The port to listen on
	 */
	UDPSocketClient(std::string inAddress, int inPort);

	/**
	 * Deconstructor
	 */
	~UDPSocketClient();

	/**
	 * Whether or not the client is connected to the server.
	 *
	 * @return		Boolean whether or not the client is connected to the server
	 */
	int isConnected();

	/**
	 * Read bytes of data from the port.
	 * TODO Provide an asynchronous version of the function
	 *
	 * @param buffer	The buffer to read data to
	 * @param maxlen	The maximum number of bytes to read
	 * @return		The number of bytes read
	 */
	int read(char * outBuffer, unsigned int maxlen);

	/**
	 * Write bytes of data to the port.
	 *
	 * @param buffer	The buffer to write data from
	 * @param len		The number of bytes to write
	 * @return		Whether or not the write was successful
	 */
	int write(const char * buffer, unsigned int len);

	/**
	 * Close the connection.
	 */
	void close();
};

#ifndef _UDP_SOCKET_SERVER
#define _UDP_SOCKET_SERVER

#ifdef _WIN32
#	define _WINSOCK_DEPRECATED_NO_WARNINGS
#	include <WinSock2.h>
#	include <WS2tcpip.h>
#	include <Windows.h>
#else
#	include <unistd.h>
#	include <arpa/inet.h>
#	include <sys/socket.h>
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
#endif

#include <string>
#include <functional>
#include <thread>
#include <queue>
#define DEFAULT_BUFFER_SIZE 4096
#define SOCKET_READ_TIMEOUT_SEC 1
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)

/**
* Asynchronous UDP socket server
*
* Refer to the following link for Microsoft's Server C++ code:
* https://msdn.microsoft.com/en-us/library/windows/desktop/ms737593(v=vs.85).aspx
*/
class UDPSocketServer {
private:
	/**
	* The address to bind to
	*/
	std::string address;

	/**
	* The port to listen on
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
	int listening;

	/**
	* Internal message to close
	*/
	volatile int closeMessage;

	/**
	* Continuously handle received data (done in a new thread)
	*
	* @param handler	The function to handle data
	*					Note, the first parameter passed to the handler
	*					will be available only within the call to the
	*					handler, otherwise it may be deleted at any time.
	*/
	void handle(std::function<void(char *, unsigned int)> handler);

public:
	/**
	* Initialize a new UDP socket server.
	*
	* @param address	The address to bind to
	* @param port		The port to listen on
	*/
	UDPSocketServer(std::string inAddress, int inPort);

	/**
	* Deconstructor
	*/
	~UDPSocketServer();

	/**
	* Whether or not the server is listening.
	*
	* @return		Boolean whether or not the server is listening
	*/
	int isListening();

	/**
	* Start the server and begin listening.
	*
	* @param handler	The function to handle data
	*/
	void server(std::function<void(char *, unsigned int)> handler);

	/**
	* Close the server and stop listening.
	*/
	void close();
};

#endif
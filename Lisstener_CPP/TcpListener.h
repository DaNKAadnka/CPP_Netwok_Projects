#pragma once

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE (49152)

// Forward declaration of class
class TcpListener;

// Callback to data recieved
typedef void (*MessageRecievedHandler) (TcpListener* listener, int socketId, std::string message);

class TcpListener
{
public:

	TcpListener(std::string ipAdress, int port, MessageRecievedHandler handler);

	~TcpListener();

	// Send message to specified client
	void Send(int client_socket, std::string msg);

	// Initialize winsocket
	bool Init();

	// Main processing loop
	void Run();

	void CleanUp();


private:

	std::string				m_ipAdress;
	int						m_port;
	MessageRecievedHandler	m_handler;
	
	
	// Create socket
	SOCKET CreateSocket();

	// Wait for connection
	SOCKET WaitForConnection(SOCKET server_socket);

};
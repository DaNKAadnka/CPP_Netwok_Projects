#pragma once

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>
#include <mutex>
#include <queue>

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

	// Server work with capability of multiple cliets handling
	void MultipleRun();

	void CleanUp();

	// Function for first thread
	void accept_connections_loop(SOCKET server_socket);

	// Function for second thread
	void recieve_data_loop(SOCKET server_socket, SOCKET client_socket);

	void process_data_loop(SOCKET server_socket);

private:

	std::string				m_ipAdress;
	int						m_port;
	MessageRecievedHandler	m_handler;
	
	
	// Create socket
	SOCKET CreateSocket();

	// Mutex for two threads
	std::mutex process_mutex;

	//Threads for accept and recieve data
	std::thread recieve_data;
	std::thread accept_connections;

	// Common data
	std::vector < SOCKET* > clients;

	std::queue < std::pair <SOCKET*, std::string* > > message_queue;
	
	//SOCKET server_socket;

	// Wait for connection
	SOCKET WaitForConnection(SOCKET server_socket);



};

struct C_message {
	std::unique_ptr<SOCKET> client_socket;
	std::string message;

	C_message(SOCKET socket, std::string mess) {
		std::unique_ptr<SOCKET> socket_ptr = std::make_unique<SOCKET> (socket);

	}
};
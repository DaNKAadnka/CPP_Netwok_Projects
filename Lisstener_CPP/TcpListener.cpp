#include "TcpListener.h"
#include <iostream>
#include <vector>

TcpListener::TcpListener(std::string ipAdress, int port, MessageRecievedHandler handler)
	: m_ipAdress(ipAdress), m_port(port), m_handler(handler)
{

}

TcpListener::~TcpListener() {
	CleanUp();
}

// Send message to specified client
void TcpListener::Send(int client_socket, std::string msg) {
	send(client_socket, msg.c_str(), msg.size() + 1, 0);
}

// Initialize winsocket
bool TcpListener::Init() {

	WSADATA wsdata;

	int wser = WSAStartup(MAKEWORD(2, 2), &wsdata);

	return (wser == 0);
}

// Main processing loop
void TcpListener::Run() {

	std::vector <char> buff(MAX_BUFFER_SIZE);

	while (true) {

		// Create a listening socket
		SOCKET server_socket = CreateSocket();
		if (server_socket == INVALID_SOCKET) {
			std::cout << "Some troubles with starting server" << std::endl;
			return;
		}

		// Wait for any connection
		SOCKET client_socket = WaitForConnection(server_socket);
		if (client_socket == INVALID_SOCKET) {
			std::cout << "Some troubles with client connection" << std::endl;
			continue;
		}

		// Start loop with recv and send

		closesocket(server_socket);

		int bytes = 0;
		std::cout << "Client found. Waiting for message.\n";

		do {
			ZeroMemory(buff.data(), buff.size());

			bytes = recv(client_socket, buff.data(), buff.size(), 0);

			// If we had some data, we should use callback
			if (bytes > 0 && m_handler != NULL) {
				m_handler(this, client_socket, std::string(buff.data(), 0, bytes));
			}
		} while (bytes > 0);
		
		closesocket(client_socket);
		

		std::cout << "Stop handling the client.\n";
	}
}

void TcpListener::CleanUp() {
	WSACleanup();
}

// Create socket
SOCKET TcpListener::CreateSocket() {

	int erStat;

	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		std::cout << "Socket initialize error!\n";
		return server_socket;
	}

	sockaddr_in socket_addr;
	ZeroMemory(&socket_addr, sizeof(socket_addr));
	
	in_addr ip_address;
	erStat = inet_pton(AF_INET, m_ipAdress.c_str(), &ip_address);

	if (erStat <= 0) {
		std::cout << "Ip Adress is invalid!\n";
		return INVALID_SOCKET;
	}

	socket_addr.sin_addr	= ip_address;
	socket_addr.sin_port	= htons(m_port);
	socket_addr.sin_family	= AF_INET;

	erStat = bind(server_socket, (sockaddr*)&socket_addr, sizeof(socket_addr));
	if (erStat == SOCKET_ERROR) {
		std::cout << "Binding error!\n";
		return INVALID_SOCKET;
	}

	erStat = listen(server_socket, SOMAXCONN);
	if (erStat == SOCKET_ERROR) {
		std::cout << "Listening error!\n";
		return INVALID_SOCKET;
	}

	// Finally, correct socket
	return server_socket;
}

// Wait for connection
SOCKET TcpListener::WaitForConnection(SOCKET server_socket) {

	sockaddr_in client_info;
	ZeroMemory(&client_info, sizeof(client_info));

	int client_info_size = sizeof(client_info);

	SOCKET client_socket = accept(server_socket, (sockaddr*) &client_info, &client_info_size);
	return client_socket;
}
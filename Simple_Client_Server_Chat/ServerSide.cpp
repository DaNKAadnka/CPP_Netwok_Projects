// ClientServerApp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

void quick_close(SOCKET& sock) {
	closesocket(sock);
	WSACleanup();
}

int main()
{
	const short BUFF_SIZE = 1024;

	//Initialize information about socket
	WSADATA wsData;

	int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0) {
		std::cout << "Error WinSock version initializaion #";
		std::cout << WSAGetLastError();
		return 1;
	}
	else {
		std::cout << "WinSock initialization is OK" << std::endl;
	}

	//Initialize socket
	SOCKET serv_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (serv_socket == INVALID_SOCKET) {
		std::cout << "Socket initialization error\n" << WSAGetLastError() << std::endl;
		/*closesocket(serv_socket);
		WSACleanup();*/
		quick_close(serv_socket);
		return 1;

	}
	else {
		std::cout << "Socket initialization success!" << std::endl;
	}

	//Initialize network information for our socket
	sockaddr_in serv_info;
	ZeroMemory(&serv_info, sizeof(serv_info));

	// IP adress for socket
	in_addr ip_sock;
	erStat = inet_pton(AF_INET, "127.0.0.1", &ip_sock);
	if (erStat <= 0) {
		std::cout << "Error in IP translation to specific format" << std::endl;
		quick_close(serv_socket);
		return 1;
	}

	// Socket network info
	serv_info.sin_family = AF_INET;
	serv_info.sin_port = htons(1234);
	serv_info.sin_addr = ip_sock;

	erStat = bind(serv_socket, (sockaddr*)&serv_info, sizeof(serv_info));
	if (erStat != 0) {
		std::cout << "Error socket binding to server \n" << WSAGetLastError() << std::endl;
		quick_close(serv_socket);
		return 1;
	}
	else {
		std::cout << "Socket binding success\n";
	}

	// Start to listen any sockets

	erStat = listen(serv_socket, SOMAXCONN);

	if (erStat != 0) {
		std::cout << "Error start to listen to sockets " << WSAGetLastError << std::endl;
		quick_close(serv_socket);
		return 1;
	}
	else {
		std::cout << "Listening... " << std::endl;
	}

	sockaddr_in client_info;
	ZeroMemory(&client_info, sizeof(client_info));

	int client_info_size = sizeof(client_info);

	SOCKET client_socket = accept(serv_socket, (sockaddr*)&client_info, &client_info_size);
	if (client_socket == INVALID_SOCKET) {
		std::cout << "Client detected, but connection is lost. " << WSAGetLastError() << std::endl;
		closesocket(client_socket);
		quick_close(serv_socket);
		return 1;
	}
	else {
		std::cout << "Connection to client established successfully!" << std::endl;
	}

	system("pause");


	// Exchange messages
	std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);							// Creation of buffers for sending and receiving data
	short packet_size = 0;												// The size of sending / receiving packet in bytes

	while (true) {
		packet_size = recv(client_socket, servBuff.data(), servBuff.size(), 0);					// Receiving packet from client. Program is waiting (system pause) until receive
		std::cout << "Client's message: " << servBuff.data() << std::endl;

		std::cout << "Your (host) message: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		// Check whether server would like to stop chatting 
		if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
			shutdown(client_socket, SD_BOTH);
			closesocket(serv_socket);
			closesocket(client_socket);
			WSACleanup();
			return 0;
		}

		packet_size = send(client_socket, clientBuff.data(), clientBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
			closesocket(serv_socket);
			closesocket(client_socket);
			WSACleanup();
			return 1;
		}

	}

	// Close our socket
	/*closesocket(serv_socket);
	WSACleanup();*/
	quick_close(serv_socket);


}
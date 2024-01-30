// OnlyClientSide.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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

int main() {

    const short BUFF_SIZE = 1024;

    WSADATA wsData;

    int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (erStat != 0) {
        std::cout << "Error WinSock version initialization\n" << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        return 1;
    }
    else {
        std::cout << "WinSock initialization is OK" << std::endl;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cout << "Socket initialization error" << std::endl;
        quick_close(client_socket);
        return 1;
    }
    else {
        std::cout << "Socket initialization success" << std::endl;
    }

    sockaddr_in  serv_info;
    ZeroMemory(&serv_info, sizeof(serv_info));

    in_addr ip_serv_sock;
    erStat = inet_pton(AF_INET, "127.0.0.1", &ip_serv_sock);
    if (erStat <= 0) {
        std::cout << "Error in IP translation to specific format" << std::endl;
        quick_close(client_socket);
        return 1;
    }

    serv_info.sin_family = AF_INET;
    serv_info.sin_port = htons(1234);
    serv_info.sin_addr = ip_serv_sock;

    erStat = connect(client_socket, (sockaddr*)&serv_info, sizeof(serv_info));
    if (erStat != 0) {
        std::cout << "Connection to server is FAILED. " << WSAGetLastError() << std::endl;
        quick_close(client_socket);
        return 1;
    }
    else {
        std::cout << "Connected to server" << std::endl;
    }


    //Exchange text data between Server and Client. Disconnection if a Client send "xxx"

    std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);							// Buffers for sending and receiving data
    short packet_size = 0;												// The size of sending / receiving packet in bytes

    while (true) {

        std::cout << "Your (Client) message to Server: ";
        fgets(clientBuff.data(), clientBuff.size(), stdin);

        // Check whether client like to stop chatting 
        if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
            shutdown(client_socket, SD_BOTH);
            closesocket(client_socket);
            WSACleanup();
            return 0;
        }

        packet_size = send(client_socket, clientBuff.data(), clientBuff.size(), 0);

        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(client_socket);
            WSACleanup();
            return 1;
        }

        packet_size = recv(client_socket, servBuff.data(), servBuff.size(), 0);

        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
            closesocket(client_socket);
            WSACleanup();
            return 1;
        }
        else
            std::cout << "Server message: " << servBuff.data() << std::endl;

    }

    closesocket(client_socket);
    WSACleanup();

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

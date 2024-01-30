#include <iostream>

#include "TcpListener.h"

void Listener_MessageRecieved(TcpListener* listener, int socketId, std::string message);

int main() 
{

	TcpListener server ("127.0.0.1", 1234, Listener_MessageRecieved);

	if (server.Init()) {
		server.Run();
	}

}

void Listener_MessageRecieved(TcpListener* listener, int client, std::string message)
{
	listener->Send(client, message);
}
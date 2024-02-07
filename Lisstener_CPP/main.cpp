#include <iostream>

#include "TcpListener.h"
#include "quoter.h"

void Listener_MessageRecieved(TcpListener* listener, int socketId, std::string message);

Quoter wisdomer("wisdom.txt");

int main() 
{

	TcpListener server ("127.0.0.1", 1234, Listener_MessageRecieved);

	if (server.Init()) {
		server.MultipleRun();
	}

}

void Listener_MessageRecieved(TcpListener* listener, int client, std::string message)
{
	//std::cout << message << std::endl;
	if (message == "Command: Quote\n") {
		listener->Send(client, wisdomer.GetRandom());
	}
	else {
		listener->Send(client, message);
	}
}
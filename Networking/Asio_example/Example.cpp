#include <iostream>
#include <chrono>

#ifndef _WIN32
#define _WIN32_WINT 0x0A00
#endif // !_WIN32


#define ASIO_STANDALONE
//asio
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector <char> vbuffer(1 * 1024);

void grab_some_data(asio::ip::tcp::socket &sock) {

	sock.async_read_some(asio::buffer(vbuffer.data(), vbuffer.size()),
		[&] (std::error_code ec, size_t length){
			if (!ec) {
				std::cout << "Read " << length << " bytes\n";
				for (int i = 0; i < length; i++) {
					std::cout << vbuffer[i];
				}
				std::cout << "\n\n";
				grab_some_data(sock);
			}
		});

}

int main() {

	setlocale(LC_ALL, "Russian");
	SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

	asio::error_code ec;

	// Create "context" - some library specific interface
	asio::io_context context;

	// Give this context some fake work
	asio::io_context::work idle_work(context);

	//Start thread
	std::thread context_thread = std::thread([&]() {context.run(); });

	std::string ch_ip;
	const std::string example_com_ip = "93.184.216.34";
	const std::string another_com_ip = "51.38.81.49";
	const std::string local_machine_ip = "127.0.0.1";

	int type;
	std::cout << "Choose IP for connection\n";
	std::cout << "1) 93.184.216.34 (example.com)\n";
	std::cout << "2) 127.0.0.1 (Local machine)\n";
	std::cout << "3) " + another_com_ip + " (Some site)\n";
	std::cout << "Your choice: ";
	std::cin >> type;
	
	switch (type)
	{
	case 1:
		ch_ip = example_com_ip;
		break;
	case 2:
		ch_ip = local_machine_ip;
		break;
	case 3:
		ch_ip = another_com_ip;
		break;
	default:
		std::cout << "Nothing\n";
		return 0;
	}

	// Initialization of endpoint which we want to access
	asio::ip::tcp::endpoint endpt(asio::ip::make_address(ch_ip, ec), 80);

	// Initialize TCP socket 
	asio::ip::tcp::socket sock(context);

	// Try to connect socket with endpoint
	sock.connect(endpt, ec);

	if (!ec) {
		std::cout << "Connected successfully!" << std::endl;
	}
	else {
		std::cout << "Failed to connect to endpoint: \n" << ec.message() << std::endl;
	}

	if (sock.is_open()) {
		
		grab_some_data(sock);
		
		std::string s_req =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";
		std::cout << s_req << std::endl;

		sock.write_some(asio::buffer(s_req.data(), s_req.size()), ec);
		
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(2000ms);
	}

	system("pause");
	//main();
	return 0;
}
#include "pch.h"
#include "Client.h"

Client::Client()
{
	// we connect to server that uses TCP. thats why SOCK_STREAM & IPPROTO_TCP
	_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_clientSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");

}

Client::~Client()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(_clientSocket);
	}
	catch (...) {}
}


void Client::connect(std::string serverIP, int port)
{

	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen to
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr =  inet_addr(serverIP.c_str());    // the IP of the server

	// notice that we step out to the global namespace
	// for the resolution of the function socket
	int status = ::connect(_clientSocket, (struct sockaddr*)&sa, sizeof(sa));

	if (status == INVALID_SOCKET)
		throw std::exception("Cant connect to server");
}

void Client::write(const std::string& message) const
{
	send(_clientSocket, message.c_str(), message.size(), 0);
}

std::string Client::read() const
{
	char output[SOCK_BUFF_SIZE] = { 0 };
	
	recv(_clientSocket, output, SOCK_BUFF_SIZE - 1, 0);

	return output;
}

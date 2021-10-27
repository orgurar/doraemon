#pragma once
#include "pch.h"

class Client
{
public:
	Client();
	~Client();
	void connect(std::string serverIP, int port);

	// read & write socket wrappers
	void write(const std::string& message) const;
	std::string read() const;

private:
	SOCKET _clientSocket;
};
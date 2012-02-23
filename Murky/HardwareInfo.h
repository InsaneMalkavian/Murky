#ifndef HWINFO_H
#define HWINFO_H

#include <string>
using namespace std;
#define _WIN32_DCOM
#include <Wbemidl.h>

# pragma comment(lib, "wbemuuid.lib")

struct Videocard {
	wstring				Caption;
	wstring				Description;
	wstring				Name;
	wstring				DriverVersion;
	wstring				VideoProcessor;
	UINT				AdapterRAM;
};

struct CPU {
	wstring				Caption;
	wstring				Manufacturer;
	wstring				Name;
	UINT				NumberOfCores;
	UINT				NumberOfLogicalProcessors;
	UINT				Family;
	UINT				Level;
	UINT				CurrentClockSpeed;
	UINT				MaxClockSpeed;
	DWORD				RegClockSpeed;
};

class HWInformation {
public:
	HWInformation();
	~HWInformation();
	Videocard			Video;
	MEMORYSTATUSEX		RAMStat;
	OSVERSIONINFOEX		OSVersion;
	CPU					CPUInfo;
private:
	HRESULT				hRes;
	IWbemLocator*		pLoc;
	IWbemServices*		pSvc;	

public:
	void Update();
	
	void GetVideoInfo(wstring &output);
	void GetCPUInfo(wstring &output);
	void GetMemInfo(wstring &output);
	int GetOSInfo(wstring &output);
	int GetAllInfo(wstring &output);

private:
	int Initialize();

};

#endif // HWINFO_H
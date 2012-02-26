#include "HardwareInfo.h"
#include <sstream>
#include <tchar.h>
#include <strsafe.h>
#include <comdef.h>

#define BUFSIZE 256
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);


HWInformation::HWInformation() {
	ZeroMemory(this, sizeof(HWInformation));
	Initialize();
	Update();
}

HWInformation::~HWInformation() {
	pSvc->Release();
	pLoc->Release();
}

void HWInformation::Update() {
	// Retrieving information about CPU
	IEnumWbemClassObject* pEnumerator = NULL;
    hRes = pSvc->ExecQuery(
        bstr_t("WQL"), 
        bstr_t("SELECT * FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
        NULL,
        &pEnumerator);
	if (FAILED(hRes)) { pSvc->Release();pLoc->Release(); CoUninitialize(); return;}
	
	IWbemClassObject *pclsObj;
    ULONG uReturn = 0;
   
    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if(0 == uReturn) {break;}

        VARIANT vtProp;

        // Get values
        hr = pclsObj->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
        CPUInfo.NumberOfCores=vtProp.uintVal;
		hr = pclsObj->Get(L"NumberOfLogicalProcessors", 0, &vtProp, 0, 0);
        CPUInfo.NumberOfLogicalProcessors=vtProp.uintVal;
		hr = pclsObj->Get(L"Family", 0, &vtProp, 0, 0);
        CPUInfo.Family=vtProp.uintVal;
		hr = pclsObj->Get(L"Level", 0, &vtProp, 0, 0);
        CPUInfo.Level=vtProp.uintVal;
		hr = pclsObj->Get(L"CurrentClockSpeed", 0, &vtProp, 0, 0);
        CPUInfo.CurrentClockSpeed=vtProp.uintVal;
		hr = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
        CPUInfo.MaxClockSpeed=vtProp.uintVal;
		hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
		CPUInfo.Caption.assign(vtProp.bstrVal);
        hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
		CPUInfo.Manufacturer.assign(vtProp.bstrVal);
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		CPUInfo.Name.assign(vtProp.bstrVal);
        VariantClear(&vtProp);
		DWORD flags=0x0000ffff;
		DWORD pf, pf2;
		DWORD lClock=0;
		long q = RegGetValue(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"~MHz", flags, &pf2, &lClock, &pf);
		CPUInfo.RegClockSpeed=lClock;
        
    }

	// Retrieving information about Videocard
	hRes = pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_VideoController"), // set new query
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	if (FAILED(hRes)) { pSvc->Release();pLoc->Release(); CoUninitialize(); return;}
	
	while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if(0 == uReturn) {break;}

        VARIANT vtProp;
		hr = pclsObj->Get(L"AdapterRAM", 0, &vtProp, 0, 0);
        Video.AdapterRAM=vtProp.uintVal;
		hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
		Video.Caption.assign(vtProp.bstrVal);
        hr = pclsObj->Get(L"Description", 0, &vtProp, 0, 0);
		Video.Description.assign(vtProp.bstrVal);
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		Video.Name.assign(vtProp.bstrVal);
        hr = pclsObj->Get(L"DriverVersion", 0, &vtProp, 0, 0);
		Video.DriverVersion.assign(vtProp.bstrVal);
        hr = pclsObj->Get(L"VideoProcessor", 0, &vtProp, 0, 0);
		Video.VideoProcessor.assign(vtProp.bstrVal);
		}
	// release objects
	pclsObj->Release();
	pEnumerator->Release();

	// retrieving information about RAM
	RAMStat.dwLength = sizeof (RAMStat);
	GlobalMemoryStatusEx (&RAMStat);

  
	}

int HWInformation::Initialize() {
	// Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------
	hRes =  CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hRes)) return -1;

	// Step 2: --------------------------------------------------
    // Set general COM security levels --------------------------
    // Note: If you are using Windows 2000, you need to specify -
    // the default authentication credentials for a user by using
    // a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
    // parameter of CoInitializeSecurity ------------------------
	hRes =  CoInitializeSecurity(NULL, 
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities 
        NULL                         // Reserved
        );
	if (FAILED(hRes)) { CoUninitialize(); return -1;}

	// Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------
	pLoc = NULL;

    hRes = CoCreateInstance(
        CLSID_WbemLocator,             
        0, 
        CLSCTX_INPROC_SERVER, 
        IID_IWbemLocator, (LPVOID *) &pLoc);
	if (FAILED(hRes)) { CoUninitialize(); return -1;}

	// Step 4: -----------------------------------------------------
    // Connect to WMI through the IWbemLocator::ConnectServer method
    pSvc = NULL;
	
    // Connect to the root\cimv2 namespace with
    // the current user and obtain pointer pSvc
    // to make IWbemServices calls.
    hRes = pLoc->ConnectServer(
         _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
         NULL,                    // User name. NULL = current user
         NULL,                    // User password. NULL = current
         0,                       // Locale. NULL indicates current
         NULL,                    // Security flags.
         0,                       // Authority (e.g. Kerberos)
         0,                       // Context object 
         &pSvc                    // pointer to IWbemServices proxy
         );
	if (FAILED(hRes)) { pLoc->Release(); CoUninitialize(); return -1;}

	// Step 5: --------------------------------------------------
    // Set security levels on the proxy -------------------------
    hRes = CoSetProxyBlanket(
       pSvc,                        // Indicates the proxy to set
       RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
       RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
       NULL,                        // Server principal name 
       RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
       RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
       NULL,                        // client identity
       EOAC_NONE                    // proxy capabilities 
    );
	if (FAILED(hRes)) { pSvc->Release();pLoc->Release(); CoUninitialize(); return -1;}
	return 0;
	}

void HWInformation::GetVideoInfo(wstring &output) {
	output.clear();
	output.append(L"Caption = " + Video.Caption);
	output.append(L"\nDescription = " + Video.Description);
	output.append(L"\nName = " + Video.Name);
	output.append(L"\nDriverVersion = " + Video.DriverVersion);
	output.append(L"\nVideoProcessor = " + Video.VideoProcessor);
	output.append(L"\nVRAM = ");
	std::wstringstream streamVal;
	streamVal<<Video.AdapterRAM/1024/1024;
	output.append(streamVal.str());
	output.append(L" MBytes");
	}

void HWInformation::GetCPUInfo(wstring &output) {
	std::wstringstream streamVal;
	output.clear();
	output.append(L"Caption = " + CPUInfo.Caption);
	output.append(L"\nName = " + CPUInfo.Name);
	output.append(L"\nManufacturer = " + CPUInfo.Manufacturer);
	streamVal<<L"\nCores = "<<CPUInfo.NumberOfCores<<L"("<<CPUInfo.NumberOfLogicalProcessors<<L")\nFamily/Level = "<<CPUInfo.Family<<L"/"<<CPUInfo.Level
		<<L"\nClocks (Cur/Max/Reg) = "<<CPUInfo.CurrentClockSpeed<<L"/"<<CPUInfo.MaxClockSpeed<<L"/"<<CPUInfo.RegClockSpeed<<" Mhz";
	output.append(streamVal.str());
	}

void HWInformation::GetMemInfo(wstring &output) {
	std::wstringstream streamVal;
	streamVal<<L"Total Physycal = "<<RAMStat.ullTotalPhys<<L"\nFree Physycal = "<<RAMStat.ullAvailPhys<<L"\nTotal Pagefile = "<<RAMStat.ullTotalPageFile<<
		L"\nFree Pagefile = "<<RAMStat.ullAvailPageFile<<L"\nTotal Virtual = "<<RAMStat.ullTotalVirtual<<L"\nFree Virtual = "<<RAMStat.ullAvailVirtual<<
		L"\nUsage = "<<RAMStat.dwMemoryLoad<<L"%";
	output.assign(streamVal.str());
	}

int HWInformation::GetOSInfo(wstring &output) {
	TCHAR pszOS[BUFSIZE];
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	PGPI pGPI;
	BOOL bOsVersionInfoEx;
	DWORD dwType;
	
	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
		return 1;
	
	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.	
	pGNSI = (PGNSI) GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")),  "GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);
	
	if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && osvi.dwMajorVersion > 4 ) {
		StringCchCopy(pszOS, BUFSIZE, TEXT("Microsoft "));
		// Test for the specific product.
		
		if ( osvi.dwMajorVersion == 6 ) {
			if( osvi.dwMinorVersion == 0 ) {
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));
				}
			if ( osvi.dwMinorVersion == 1 ) {
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
				}
			pGPI = (PGPI) GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")), 
				"GetProductInfo");
			
			pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);
			switch( dwType ) {
			case PRODUCT_ULTIMATE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate Edition" ));
				break;
            //case PRODUCT_PROFESSIONAL:
              // StringCchCat(pszOS, BUFSIZE, TEXT("Professional" ));
               //break;
            case PRODUCT_HOME_PREMIUM:
               StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium Edition" ));
               break;
            case PRODUCT_HOME_BASIC:
               StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic Edition" ));
               break;
            case PRODUCT_ENTERPRISE:
               StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
               break;
            case PRODUCT_BUSINESS:
               StringCchCat(pszOS, BUFSIZE, TEXT("Business Edition" ));
               break;
            case PRODUCT_STARTER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Starter Edition" ));
               break;
            case PRODUCT_CLUSTER_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server Edition" ));
               break;
            case PRODUCT_DATACENTER_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition" ));
               break;
            case PRODUCT_DATACENTER_SERVER_CORE:
               StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition (core installation)" ));
               break;
            case PRODUCT_ENTERPRISE_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
               break;
            case PRODUCT_ENTERPRISE_SERVER_CORE:
               StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition (core installation)" ));
               break;
            case PRODUCT_ENTERPRISE_SERVER_IA64:
               StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
               break;
            case PRODUCT_SMALLBUSINESS_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server" ));
               break;
            case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
               StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium Edition" ));
               break;
            case PRODUCT_STANDARD_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition" ));
               break;
            case PRODUCT_STANDARD_SERVER_CORE:
               StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition (core installation)" ));
               break;
            case PRODUCT_WEB_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Web Server Edition" ));
               break;
				}
			}
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 ) {
			if( GetSystemMetrics(SM_SERVERR2) )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Server 2003 R2, "));
			else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Storage Server 2003"));
			//else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
			// StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Home Server"));
			else if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64) {
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows XP Professional x64 Edition"));
				}
			else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003, "));
			
			// Test for the server type.
			if ( osvi.wProductType != VER_NT_WORKSTATION ) {
				if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 ) {
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition for Itanium-based Systems" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition for Itanium-based Systems" ));
					}
				else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 ) {
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter x64 Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise x64 Edition" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard x64 Edition" ));
					}
				else {
					if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Compute Cluster Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition" ));
					else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Web Edition" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard Edition" ));
					}
				}
			}
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 ) {
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP "));
			if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Home Edition" ));
			else StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
			}
		
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 ) {
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));			
			if ( osvi.wProductType == VER_NT_WORKSTATION ) {
				StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
				}
			else  {
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Server" ));
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Advanced Server" ));
				else StringCchCat(pszOS, BUFSIZE, TEXT( "Server" ));
				}
			}
		
		// Include service pack (if any) and build number.
		if( _tcslen(osvi.szCSDVersion) > 0 ) {
			StringCchCat(pszOS, BUFSIZE, TEXT(" ") );
			StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
			}
		
		TCHAR buf[80];
		StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
		StringCchCat(pszOS, BUFSIZE, buf);
		
		if ( osvi.dwMajorVersion >= 6 ) {
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				StringCchCat(pszOS, BUFSIZE, TEXT( ", 64-bit" ));
			else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				StringCchCat(pszOS, BUFSIZE, TEXT(", 32-bit"));
			}
		output.assign(pszOS);
		return 1; 
		}
	else {
		output.assign(L"Unknown version of Windows");
		return 0;
		}
}

int HWInformation::GetAllInfo(wstring &output) {
	wstring temp;
	output.append(L"System information:\n");
	output.append(L"CPU info:\n");
	GetCPUInfo(temp);
	output.append(temp);
	output.append(L"\nVideocard info:\n");
	GetVideoInfo(temp);
	output.append(temp);
	output.append(L"\nMemory info:\n");
	GetMemInfo(temp);
	output.append(temp);
	output.append(L"\nOS info:\n");
	int error = GetOSInfo(temp);
	output.append(temp);
	return error;
	}

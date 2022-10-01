#include "ServicePrototype.h"
#include "ServiceLog.h"
#include <assert.h>
#include <strsafe.h>

ServicePrototype* ServicePrototype::service_ = NULL;

BOOL ServicePrototype::serviceRun(ServicePrototype& service)
{
	service_ = &service;
	SERVICE_TABLE_ENTRY serviceTable[] =
	{
		{ service.serviceName_, serviceMain },
		{ NULL, NULL }
	};
	return StartServiceCtrlDispatcher(serviceTable);
}

void WINAPI ServicePrototype::serviceCtrlHandler(DWORD dwCtrl)
{
	switch (dwCtrl)
	{
	case SERVICE_CONTROL_STOP: service_->serviceStop(); break;
	case SERVICE_CONTROL_PAUSE: service_->servicePause(); break;
	case SERVICE_CONTROL_CONTINUE: service_->serviceContinue(); break;
	case SERVICE_CONTROL_SHUTDOWN: service_->serviceShutdown(); break;
	case SERVICE_CONTROL_INTERROGATE: break;
	default: break;
	}
}

void WINAPI ServicePrototype::serviceMain(DWORD dwArgc, LPWSTR* pszArgv)
{
	if (service_ == NULL) {
		LOG_ERROR << "Service pointer is NULL: " << GetLastError();
		return;
	}

	//Register the handler function for the service
	service_->serviceStatusHande_ = RegisterServiceCtrlHandler(service_->serviceName_, serviceCtrlHandler);
	if (service_->serviceStatusHande_ == NULL) {
		LOG_ERROR << "Service status handle error: " << GetLastError();
		return;
	}

	// Start the service
	service_->serviceStart(dwArgc, pszArgv);
}

ServicePrototype::ServicePrototype(LPWSTR serviceName, BOOL canStop, BOOL canShutDown, BOOL canPauseContinue)
{
	serviceName_ = (serviceName == NULL) ? const_cast<LPWSTR>(L"Basic_Service") : serviceName;
	serviceStatusHande_ = NULL;

	// The phrase "WIN32" applies to the name of the api layer, it doesn't have anything to do with the bitness of the process. 
	// Win32 was the successor to the 16-bit api used in Windows 3.x, one of the three sub-systems supported on NT (Posix and OS/2 are the forgotten ones).
	// It is a source of confusion after the winapi got ported to 64 - bit, the reason that the[win32] tag on SO now redirects to[winapi].
	// Creating 64 - bit services isn't different. Just add the "x64" Platform target on your project and recompile.
	serviceStatus_.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

	serviceStatus_.dwCurrentState = SERVICE_START_PENDING;
	DWORD dwControlsAccepted = 0;
	if (canStop)
		dwControlsAccepted |= SERVICE_ACCEPT_STOP;
	if (canShutDown)
		dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
	if (canPauseContinue)
		dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus_.dwControlsAccepted = dwControlsAccepted;
	serviceStatus_.dwWin32ExitCode = NO_ERROR;
	serviceStatus_.dwServiceSpecificExitCode = 0;
	serviceStatus_.dwCheckPoint = 0;
	serviceStatus_.dwWaitHint = 0;

	WCHAR szPath[MAX_PATH];
	if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)) == 0) {
		wprintf(L"GetModuleFileName failed w/err 0x%08lx\n", GetLastError());
	}
	std::wstring path(szPath);
	path = path.substr(0, path.rfind('\\'));
	logger::init(path + "\\Log");
}

ServicePrototype::~ServicePrototype()
{

}

void ServicePrototype::setServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;
	serviceStatus_.dwCurrentState = dwCurrentState;
	serviceStatus_.dwWin32ExitCode = dwWin32ExitCode;
	serviceStatus_.dwWaitHint = dwWaitHint;
	serviceStatus_.dwCheckPoint = ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED)) ?
		0 : dwCheckPoint++;
	if (TRUE != SetServiceStatus(serviceStatusHande_, &serviceStatus_)) {
		LOG_ERROR << GetLastError();
	}
}

void ServicePrototype::serviceStart(DWORD dwArgc, LPWSTR* pszArgv)
{
	setServiceStatus(SERVICE_START_PENDING);
	onStart(dwArgc, pszArgv);
	setServiceStatus(SERVICE_RUNNING);
}

void ServicePrototype::serviceStop()
{
	setServiceStatus(SERVICE_STOP_PENDING);
	onStop();
	setServiceStatus(SERVICE_STOPPED);
}

void ServicePrototype::servicePause() {
	setServiceStatus(SERVICE_PAUSE_PENDING);
	onPause();
	setServiceStatus(SERVICE_PAUSED);
}

void ServicePrototype::serviceContinue() {
	setServiceStatus(SERVICE_CONTINUE_PENDING);
	onContinue();
	setServiceStatus(SERVICE_RUNNING);
}

void ServicePrototype::serviceShutdown()
{
	onShutdown();
	setServiceStatus(SERVICE_STOPPED);
}

void ServicePrototype::onStart(DWORD dwArgc, LPWSTR* pszArgv)
{

}

void ServicePrototype::onStop()
{

}

void ServicePrototype::onPause()
{

}

void ServicePrototype::onContinue()
{

}

void ServicePrototype::onShutdown()
{

}

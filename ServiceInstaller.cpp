#include <stdio.h>
#include <windows.h>
#include "ServiceLog.h"
#include "ServiceInstaller.h"

void installService(LPWSTR serviceName)
{
	WCHAR szPath[MAX_PATH];
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;

	if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)) == 0) {
		LOG_ERROR << "OpenSCManager failed w/err " << GetLastError();
		cleanUp(schSCManager, schService);
		return;
	}
	schService = CreateService(
		schSCManager,
		serviceName,
		serviceName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		szPath,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);
	if (schService == NULL) {
		LOG_ERROR << "CreateService failed w/err " << GetLastError();
		wprintf(L"CreateService failed w/err 0x%08lx\n", GetLastError());
		cleanUp(schSCManager, schService);
		return;
	}

	LOG_INFO << "Service " << serviceName << " installed";
	wprintf(L"Service installed\n");
	cleanUp(schSCManager, schService);
}

void uninstallService(LPWSTR serviceName)
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	SERVICE_STATUS ssSvcStatus = {};
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (schSCManager == NULL) {
		LOG_ERROR << "OpenSCManager failed w/err " << GetLastError();
		cleanUp(schSCManager, schService);
		return;
	}
	schService = OpenService(schSCManager, serviceName, SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
	if (schService == NULL) {
		LOG_ERROR << "OpenService failed w/err " << GetLastError();
		cleanUp(schSCManager, schService);
		return;
	}
	if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus)) {
		LOG_INFO << "Stopping " << serviceName;
		Sleep(100);
		while (QueryServiceStatus(schService, &ssSvcStatus)) {
			if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING) {
				LOG_INFO << "Stop pending...";
				Sleep(100);
			}
			else break;
		}
		if (ssSvcStatus.dwCurrentState == SERVICE_STOP)
			LOG_INFO << serviceName << " was stopped";
		else
			LOG_INFO << serviceName << " failed to stop";
	}
	if (DeleteService(schService) != TRUE) {
		LOG_ERROR << "DeleteService failed w/err " << GetLastError();
		cleanUp(schSCManager, schService);
		return;
	}
	LOG_INFO << serviceName << " was removed";
	cleanUp(schSCManager, schService);
}

void cleanUp(SC_HANDLE schSCManager, SC_HANDLE schService)
{
	LOG_INFO << "Clean up schSCManager and schService handler...";
	if (schSCManager) {
		CloseServiceHandle(schSCManager);
		schSCManager = NULL;
	}
	if (schService) {
		CloseServiceHandle(schService);
		schService = NULL;
	}
}

#include <stdio.h>
#include <windows.h>
#include "ServiceBasic.h"
#include "ServicePrototype.h"
#include "ServiceInstaller.h"
#include <iostream>
#include "ServiceLog.h"

#define SERVICE_NAME L"Test_Service"
#define WIN32_LEAN_AND_MEAN

int main(int argc, wchar_t* argv[])
{
	if ((argc > 1) && ((*argv[1] == '-') || (*argv[1] == '/'))) {
		if (wcscmp(L"install", argv[1] + 1) == 0) {
			// Install the service when command is
			// "-install" or "/install"
			installService(const_cast<LPWSTR>(SERVICE_NAME));
		}
		else if (wcscmp(L"remove", argv[1] + 1) == 0) {
			// Uninstall the service when command is
			// "-remove" or "/remove"
			uninstallService(const_cast<LPWSTR>(SERVICE_NAME));
		}
	}
	else {
		wprintf(L"Parameters:\n");
		wprintf(L" -install to install the service.\n");
		wprintf(L" -remove to remove the service.\n");

		ServiceBasic service(const_cast<LPWSTR>(SERVICE_NAME));
		if (!ServicePrototype::serviceRun(service))
			wprintf(L"Service failed to run w/err 0x%08lx\n", GetLastError());
	}
	return 0;
}
#ifndef SERVICE_INSTALLER_H
#define SERVICE_INSTALLER_H

/// <summary>
/// Command-function for service install
/// </summary>
/// <param name="serviceName">Service name to install</param>
void installService(LPWSTR serviceName);
/// <summary>
/// Command-function for service uninstall
/// </summary>
/// <param name="serviceName">Service name to remove</param>
void uninstallService(LPWSTR serviceName);
/// <summary>
/// Command-function for closing service manager's handle \ descriptor and service's handle \ descriptor
/// </summary>
/// <param name="schSCManager">Service manager's handle \ descriptor for closing</param>
/// <param name="schService">Service's handle \ descriptor for closing</param>
void cleanUp(SC_HANDLE schSCManager, SC_HANDLE schService);

#endif SERVICE_INSTALLER_H
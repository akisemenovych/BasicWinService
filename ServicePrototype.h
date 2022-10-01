#ifndef SERVICE_PROTOTYPE_H
#define SERVICE_PROTOTYPE_H

#include <windows.h>

/// <summary>
/// Prototype pattern for future services
/// </summary>
class ServicePrototype
{
public: 
	/// <summary>
	/// Constructor with input service attributes
	/// </summary>
	/// <param name="serviceName">Service name</param> 
	/// <param name="canStop">Service can stop by user demand</param>
	/// <param name="canShutDown">Service can shut down by user demand</param>
	/// <param name="canPauseContinue">Service can pause by user demand</param>
	ServicePrototype(LPWSTR serviceName, BOOL canStop = TRUE, BOOL canShutDown = TRUE, BOOL canPauseContinue = FALSE);
	/// <summary>
	/// Virtual destructor of base class
	/// </summary>
	virtual ~ServicePrototype();
	/// <summary>
	/// Command-function for service start
	/// </summary>
	/// <param name="service">Service name to start</param>
	/// <returns>Flag of success TRUE or failure FALSE</returns>
	static BOOL serviceRun(ServicePrototype& service);
	/// <summary>
	/// Command-function for service stop
	/// </summary>
	void serviceStop();

protected:
	/// <summary>
	/// Service start event handler
	/// </summary>
	/// <param name="dwArgc">Command line parameters count</param>
	/// <param name="pszArgv">Command line parameters</param>
	virtual void onStart(DWORD dwArgc, LPWSTR* pszArgv);
	/// <summary>
	/// Service stop event handler
	/// </summary>
	virtual void onStop();
	/// <summary>
	/// Service pause event handler
	/// </summary>
	virtual void onPause();
	/// <summary>
	/// Service continue event handler
	/// </summary>
	virtual void onContinue();
	/// <summary>
	/// Service shutdown event handler
	/// </summary>
	virtual void onShutdown();
	/// <summary>
	/// Service status setter for core
	/// </summary>
	/// <param name="dwCurrentState">Flag of current state</param>
	/// <param name="dwWin32ExitCode">Flag of exit code</param>
	/// <param name="dwWaitHint">The estimated time required for a pending start, stop, pause, or continue operation, in milliseconds</param>
	void setServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode = NO_ERROR, DWORD dwWaitHint = 0);

private:
	/// <summary>
	/// Service main entry-point, that will be launched by winCore after service start command
	/// </summary>
	/// <param name="dwArgc">Command line parameters count</param>
	/// <param name="lpszArgv">Command line parameters</param>
	static void WINAPI serviceMain(DWORD dwArgc, LPWSTR* lpszArgv);
	/// <summary>
	/// Function for processing events that was passed by winCore to service
	/// </summary>
	/// <param name="dwCtrl">Event code, passed by winCore</param>
	static void WINAPI serviceCtrlHandler(DWORD dwCtrl);
	/// <summary>
	/// Proccess start event
	/// </summary>
	/// <param name="dwArgc">Command line parameters count</param>
	/// <param name="pszArgv">Command line parameters</param>
	void serviceStart(DWORD dwArgc, LPWSTR* pszArgv);
	/// <summary>
	/// Proccess pause event
	/// </summary>
	void servicePause();
	/// <summary>
	/// Process continue event
	/// </summary>
	void serviceContinue();
	/// <summary>
	/// Process shutdown event
	/// </summary>
	void serviceShutdown();

private:
	/// <summary>
	/// Service pointer. Doesn't enscoped in std::unique_ptr, because creation and destruction handled on winCore side.
	/// </summary>
	static ServicePrototype* service_;
	/// <summary>
	/// Service name
	/// </summary>
	LPWSTR serviceName_;
	/// <summary>
	/// Current service status
	/// </summary>
	SERVICE_STATUS serviceStatus_;
	/// <summary>
	/// Service status handle / descriptor
	/// </summary>
	SERVICE_STATUS_HANDLE serviceStatusHande_;

};

#endif /// SERVICE_PROTOTYPE_H

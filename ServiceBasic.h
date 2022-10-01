#ifndef SERVICE_BASIC_H
#define SERVICE_BASIC_H

#include "ServicePrototype.h"
#include <thread>

/// <summary>
/// Service's class, composed from ServicePrototype base class
/// </summary>
class ServiceBasic : public ServicePrototype
{
public:
	/// <summary>
	/// Constructor with input service attributes
	/// </summary>
	/// <param name="serviceName">Service name</param> 
	/// <param name="canStop">Service can stop by user demand</param>
	/// <param name="canShutDown">Service can shut down by user demand</param>
	/// <param name="canPauseContinue">Service can pause by user demand</param>
	ServiceBasic(LPWSTR serviceName, BOOL canStop = TRUE, BOOL canShutDown = TRUE, BOOL canPauseContinue = FALSE);
	/// <summary>
	/// Current instance's destructor
	/// </summary>
	virtual ~ServiceBasic();

protected:
	/// <summary>
	/// Service start event handler
	/// </summary>
	/// <param name="dwArgc">Command line parameters count</param>
	/// <param name="pszArgv">Command line parameters</param>
	virtual void onStart(DWORD dwArgc, LPWSTR* dszArgv) final override;
	/// <summary>
	/// Service stop event handler
	/// </summary>
	virtual void onStop() final override;
	/// <summary>
	/// Service's specific task
	/// </summary>
	void serviceTask();

private:
	/// <summary>
	/// Flag, for checking if service in stop state
	/// </summary>
	BOOL stopping_;
	/// <summary>
	/// Stop event handler
	/// </summary>
	HANDLE stopEvent_;
	/// <summary>
	/// Worker thread, that busy with service's specific task
	/// </summary>
	std::thread workerThread_;
};

#endif

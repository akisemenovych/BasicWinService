#include "ServiceBasic.h"
#include "ServiceLog.h"
#include <random>

ServiceBasic::ServiceBasic(LPWSTR serviceName, BOOL canStop, BOOL canShutDown, BOOL canPauseContinue)
	: ServicePrototype(serviceName, canStop, canShutDown, canPauseContinue)
{
	stopping_ = FALSE;
	if (NULL == (stopEvent_ = CreateEvent(NULL, TRUE, FALSE, NULL)))
		LOG_ERROR << "Error in ServiceBasic construction: " << GetLastError();
}

ServiceBasic::~ServiceBasic()
{
	if (stopEvent_) {
		CloseHandle(stopEvent_);
		stopEvent_ = NULL;
	}
}

void ServiceBasic::onStart(DWORD dwArgc, LPWSTR* lpszArgv)
{
	LOG_INFO << "Starting service pending...";
	stopping_ = FALSE;
	workerThread_ = std::thread(&ServiceBasic::serviceTask, this);
}

void ServiceBasic::onStop()
{
	LOG_INFO << "Stop service pending...";
	stopping_ = TRUE;
	if (WAIT_OBJECT_0 != WaitForSingleObjectEx(stopEvent_, INFINITE)) {
		LOG_ERROR << "Error occured in onStop: " << GetLastError();
	}
	workerThread_.join();
}

void ServiceBasic::serviceTask()
{
	std::uniform_real_distribution<float> distribution(0, RAND_MAX);
	std::random_device rd;
	std::default_random_engine generator(rd()); 
	float probability = 0.0;
	while (!stopping_) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		probability = (distribution(generator) / static_cast<float>(RAND_MAX)) * 10000;
		LOG_INFO << "Executed work in worker thread: " << probability;
	}
	SetEvent(stopEvent_);
}

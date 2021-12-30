#pragma once

////////////////////////////////////////
// class CProcessorUsage;
//
// Calculates overal processor usage at
// any given time.
//
// The usage value is updated every 200
// milliseconds;
//
// The class is fully thread-safe;
//
#include <Windows.h>

class CProcessorUsage
{
	typedef BOOL (WINAPI* pfnGetSystemTimes)(LPFILETIME lpIdleTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime );
	typedef LONG (WINAPI* pfnNtQuerySystemInformation)(ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

	struct PROC_PERF_INFO
	{
		LARGE_INTEGER IdleTime;
		LARGE_INTEGER KernelTime;
		LARGE_INTEGER UserTime;
		LARGE_INTEGER Reserved1[2];
		ULONG Reserved2;
	};

public:

    CProcessorUsage();
    ~CProcessorUsage();

    float GetUsage(bool processOnly=false);
	void setInterval(int newInterval);

private:

	void GetSysTimes(__int64 & idleTime, __int64 & kernelTime, __int64 & userTime);

	////////////////////////////////////////////////
	// Set of static variables to be accessed from
	// within critical section by multiple threads:
	//
	static DWORD s_TickMark;
	static __int64 s_time;
	static __int64 s_idleTime;
	static __int64 s_kernelTime;
	static __int64 s_userTime;
	static float s_lastCpu;
	static float s_cpu[5];
	static __int64 s_kernelTimeProcess;
	static __int64 s_userTimeProcess;
	static float s_cpuProcess[5];
    static int s_count;
    static int s_index;

	unsigned int interval;
	bool firstTime;
	//
	/////////////////////////////////////////////////

	pfnGetSystemTimes s_pfnGetSystemTimes;
	pfnNtQuerySystemInformation s_pfnNtQuerySystemInformation;
	CRITICAL_SECTION m_cs;
	PROC_PERF_INFO * m_pInfo;
	ULONG m_uInfoLength;
};
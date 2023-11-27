#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#ifndef _FMOD_EXAMPLE_COMMON_PLATFORM_H_
#define _FMOD_EXAMPLE_COMMON_PLATFORM_H_


int FMOD_Main();


typedef CRITICAL_SECTION Common_Mutex;

inline void Common_Mutex_Create(Common_Mutex *mutex)
{
    InitializeCriticalSectionEx(mutex, 0, 0);
}

inline void Common_Mutex_Destroy(Common_Mutex *mutex)
{
    DeleteCriticalSection(mutex);
}

inline void Common_Mutex_Enter(Common_Mutex *mutex)
{
    EnterCriticalSection(mutex);
}

inline void Common_Mutex_Leave(Common_Mutex *mutex)
{
    LeaveCriticalSection(mutex);
}


#endif
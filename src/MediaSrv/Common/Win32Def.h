/***********************************************************************************************************************
Copyright:  
FileName: Win32Def.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef COMMON_WIN32_DEF_H_
#define COMMON_WIN32_DEF_H_
#include <process.h>
#include "../stdafx.h"
#include "BaseType.h"

//time
#define mSleep(ms)         Sleep(ms)
#define sleep(s)           Sleep(s * 1000)
#define GET_TICKS()        GetTickCount()

//Thread data type
typedef DWORD                                   tProcType_t;
typedef LPVOID                                  tProcPara_t;
typedef HANDLE                                  tHandle_t;
typedef DWORD  (*tThreadProc_p)(LPVOID);

//lock data type
typedef SRWLOCK                                 lkHandle_t;
//cond data type
typedef CONDITION_VARIABLE                      cdHandle_t;

//Thread operation
#define CREATE_NORMAL_THREAD(Proc, Param, StackSize, Out)                                                                 \
	do {                                                                                                                  \
		Out = CreateThread(NULL, StackSize, (LPTHREAD_START_ROUTINE)Proc, Param, 0, NULL);                                \
	} while (0)

#define CANCEL_NORMAL_THREAD(Handle)                                                                                      \
	do {                                                                                                                  \
		TerminateThread(Handle, 0);                                                                                       \
	} while (0)
 
#define THREAD_HANDLE_OK(h)  (NULL == h ? 0 : 1)

//Lock operation
#define LOCK_INIT(handle)                                                                                                 \
	do {                                                                                                                  \
		InitializeSRWLock(&handle);                                                                                       \
	} while (0)

#define LOCK_DESTROY(handle)                                                                                             

#define LOCK_LOCK(handle)                                                                                                 \
	do {                                                                                                                  \
		AcquireSRWLockExclusive(&handle);                                                                                 \
	} while (0)

#define LOCK_UNLOCK(handle)                                                                                               \
	do {                                                                                                                  \
		ReleaseSRWLockExclusive(&handle);                                                                                 \
	} while (0)

//COND operation
#define COND_INIT(handle)                                                                                                 \
	do {                                                                                                                  \
		InitializeConditionVariable(&handle);                                                                             \
	} while (0)

#define COND_DESTROY(handle)                                                                                              \
	do {                                                                                                                  \
	} while (0)

#define COND_WAKE(handle)                                                                                                 \
	do {                                                                                                                  \
		WakeConditionVariable(&handle);                                                                                   \
	} while (0)

#define COND_WAIT(handle,lock)                                                                                            \
	do {                                                                                                                  \
		SleepConditionVariableSRW(&handle, &lock, INFINITE, 0);                                                           \
	} while (0)


//Socket
#define Socket_t                                     SOCKET
#define SOCK_ADDR_IN                                 SOCKADDR_IN
#define SOCK_ADDR                                    SOCKADDR
#define SOCKET_ERR                                   SOCKET_ERROR
#define SOCK_CTL_CMD				                 FIONBIO
#define SOCK_CTL_NOBLOCK				             1UL
#define SOCK_CTL(x, y, z)			                 ioctlsocket((x), (y), (u_long*)(z))
#define SOCK_CLOSE(x)				                 closesocket((x))  
#define SOCK_HANDLE_OK(sHandle)                      ((S32)sHandle > 0 ? 1 : 0)
//socket errno
#define SOCK_HANDLE_NODATA                           (-10000)
#define SOCK_HANDLE_CLOSE                            (-10001)
#define SOCK_HANDLE_ERROR                            (-10002)
#define SOCK_HANDLE_BUFFULL                          (-10003)

//Err
#define SOCK_ERROR_NO                                     WSAGetLastError()

//Common define 
#define ATTRIBUTE_PACKED
#pragma pack(push)
#pragma pack(1)



#endif  /*COMMON_WIN32_DEF_H_*/
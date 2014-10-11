// SrvMonitor.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

using namespace std;
/*
#ifdef NDEBUG  
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 
#endif*/

#define EXE_FILE_NAME     "E:\\SvnClt\\MediaStream\\trunk\\MediaSrv\\Debug\\MediaSrv.exe"

#define TASK_MAX_NUM                   2
TCHAR gTaskList[TASK_MAX_NUM][MAX_PATH] = 
{
	_T("MediaSrv.exe"),
	_T("WellDVR2.exe")
};

typedef struct _TaskInfo_
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi; //进程信息：
	BOOL bRun;
	WCHAR name[MAX_PATH];
}TaskInfo_t;


//
// FindProcess
// 这个函数唯一的参数是你指定的进程名，如:你的目标进程
// 是 "Notepad.exe",返回值是该进程的ID，失败返回0
//

DWORD FindProcess(TCHAR *strProcessName)
{
	DWORD aProcesses[1024], cbNeeded, cbMNeeded;
	HMODULE hMods[1024];
	HANDLE hProcess;
	TCHAR szProcessName[MAX_PATH];

	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )  return 0;
	for(int i=0; i< (int) (cbNeeded / sizeof(DWORD)); i++)
	{
		//_tprintf(_T("%d\t"), aProcesses[i]);
		hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
		EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
		GetModuleFileNameEx( hProcess, hMods[0], szProcessName,sizeof(szProcessName));
		if(_tcsstr(szProcessName, strProcessName))
		//if(strstr(szProcessName, strProcessName))
		{
			//_tprintf(_T("%s;"), szProcessName);
			return(aProcesses[i]);
		}
		//_tprintf(_T("\n"));
	}


	return 0;
}

//
// Function: ErrorForce
// 此函数中用上面的 FindProcess 函数获得你的目标进程的ID
// 用WIN API OpenPorcess 获得此进程的句柄，再以TerminateProcess
// 强制结束这个进程
//
HANDLE GetProcessHandle(DWORD dwProcess)
{
	// When the all operation fail this function terminate the "winlogon" Process for force exit the system.
	HANDLE hYourTargetProcess = OpenProcess(PROCESS_TERMINATE|PROCESS_QUERY_INFORMATION |   // Required by Alpha
		PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
		PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
		PROCESS_VM_WRITE,             // For WriteProcessMemory
		FALSE, dwProcess);
	return hYourTargetProcess;
}

VOID KillProcess(DWORD dwProcess)
{
	HANDLE hld= GetProcessHandle(dwProcess);
	if(hld == NULL)
	{
		return;
	}

	TerminateProcess(hld, 0);
	CloseHandle(hld); 

	return;
}

VOID KillProcess(TCHAR* szProcessName)
{
	DWORD dwProcess = FindProcess(szProcessName);
	if(dwProcess != 0)
	{
		KillProcess(dwProcess);
	}
	return;
}

//
// GetDebugPriv
// 在 Windows NT/2000/XP 中可能因权限不够导致以上函数失败
// 如以　System 权限运行的系统进程，服务进程
// 用本函数取得　debug 权限即可,Winlogon.exe 都可以终止哦 :)
//
// 
// BOOL GetDebugPriv()
// {
// 	HANDLE hToken;
// 	LUID sedebugnamue;
// 	TOKEN_PRIVILEGES tkp;
// 
// 	if ( ! OpenProcessToken( GetCurrentProcess(),
// 		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
// 	{
// 		return FALSE;
// 	}
// 
// 	if( !LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnamue))
// 	//if ( ! LookupPrivilegue( NULL, SE_DEBUG_NAME, &sedebugnamue ) )
// 	{
// 		CloseHandle( hToken );
// 		return FALSE;
// 	}
// 
// 	tkp.PrivilegeCount = 1;
// 	tkp.Privileges[0].Luid = sedebugnamue;
// 	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
// 
// 	if (!AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) )
// 	{
// 		CloseHandle( hToken );
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// }

string WS2S(const wchar_t* pSrc)
{
	size_t len = wcslen(pSrc) + 1;
	size_t converted = 0;
	char *CStr =(char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, CStr, len, pSrc, _TRUNCATE);
	string strDst = CStr;
	delete[] CStr;
	CStr = NULL;
	return strDst;
}

wstring S2WS(const char* pSrc)
{
	size_t len = strlen(pSrc) + 1;
	size_t converted = 0;
	wchar_t *WStr;
	WStr=(wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, pSrc, _TRUNCATE);
	wstring strDst = WStr;
	delete[] WStr;
	WStr = NULL;
	return strDst;
}

void ShowConsole(BOOL bShow)
{
	int    nCmdShow;    
	HWND   hWnd;       
	SetConsoleTitle(TEXT("SvrMonitor")); 	
	Sleep(100);
	hWnd=::FindWindow(NULL, TEXT("SvrMonitor"));
	if (bShow)
		nCmdShow = SW_SHOWNORMAL;
	else
		nCmdShow = SW_HIDE;
	ShowWindow(hWnd, nCmdShow);
}

//mbstowcs wcstombs
string GetAppPath()
{  
	string path;
	size_t OutSize;
	char   exe1[MAX_PATH];
	WCHAR  exe2[MAX_PATH];
	GetModuleFileName(NULL, exe2, sizeof(exe2));
	if (!wcstombs_s(&OutSize, exe1, sizeof(exe1), exe2, MAX_PATH))
	{
		path = exe1;
		path = path.substr(0, path.find_last_of("\\") + 1);
	}
	
	return path;
}

string GetAppName()
{  
	string path;
	size_t OutSize;
	char   exe1[MAX_PATH];
	WCHAR  exe2[MAX_PATH];
	GetModuleFileName(NULL, exe2, sizeof(exe2));
	if (!wcstombs_s(&OutSize, exe1, sizeof(exe1), exe2, MAX_PATH))
	{
		path = exe1;
	}

	return path;
}

void AutoStart()
{
	HKEY hKey;
	LPCTSTR lpRun = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		WCHAR  exe2[MAX_PATH];
		int len = GetModuleFileName(NULL, exe2, MAX_PATH);

		if (len == 0)
		{
			DWORD dwErr = GetLastError();
		}
		else
		{
			lRet = RegSetValueEx(hKey,TEXT("SrvMonitor"), 0, REG_SZ, (BYTE*)exe2, len*2);
		}
		RegCloseKey(hKey);
	}
}

void InitTask(TaskInfo_t* t, int num)
{
	string path;
	size_t OutSize;

	for (int i = 0; i < num; i++)
	{
		memset(&t[i], 0x00, sizeof(TaskInfo_t));
		t[i].si.cb = sizeof(t[i].si); 
		t[i].si.wShowWindow = SW_HIDE;


		t[i].bRun  = false;
		path = GetAppPath();
		path += WS2S(gTaskList[i]);
		mbstowcs_s(&OutSize, t[i].name, MAX_PATH, path.c_str(), path.size());
	}
}

void InitSys()
{
	//ShowConsole(false);
	AutoStart();
}

//int _tmain(int argc, _TCHAR* argv[])
int w32_main()
{
	InitSys();

	int tIndex = 0;
	int tCount = TASK_MAX_NUM;
	TaskInfo_t t[TASK_MAX_NUM];
	HANDLE hProcess[TASK_MAX_NUM];
	InitTask(t, tCount);

	do
	{
		for (int i = 0; i < tCount; i++) 
		{
			DWORD dwFind = FindProcess(gTaskList[i]);
			if(dwFind == 0)
			{
				t[i].bRun = FALSE;
			}

			if (!t[i].bRun)
			{
				if(dwFind == 0)
				{
					t[i].bRun = CreateProcess(t[i].name, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &t[i].si, &t[i].pi);
					if (t[i].bRun)
					{
						hProcess[i] = t[i].pi.hProcess;
					}
				}
				else
				{
					t[i].bRun = TRUE;
					hProcess[i] = GetProcessHandle(dwFind);
				}
			}
		}

		DWORD rVal = WaitForMultipleObjects(tCount, hProcess, false, INFINITE);
		switch(rVal)
		{
		case WAIT_TIMEOUT:
			break;
		case WAIT_FAILED:
			break;
		default:
			tIndex = rVal - WAIT_OBJECT_0;
			t[tIndex].bRun = false;
			TerminateProcess(t[tIndex].pi.hProcess, 0);
			CloseHandle(t[tIndex].pi.hProcess); 
			CloseHandle(t[tIndex].pi.hThread);
		}
		//该sleep作用于当有错误进程或者进程起来就死时不至于跑死机器
		Sleep(1000);
	} while (true);

	return 0;
}

#if 0
#define LOGFILE           "C:\\SrvMonitor.txt" //输出文本
SERVICE_STATUS             gSrvStatus;
SERVICE_STATUS_HANDLE      ghStatus;
#define EXE_FILE_NAME      "E:\\SvnClt\\MediaStream\\trunk\\MediaSrv\\Debug\\MediaSrv.exe"  
/*
char gExeList[10] =  
{
	{"E:\\SvnClt\\MediaStream\\trunk\\MediaSrv\\Debug\\MediaSrv.exe"},
	{""},
};*/

int  WriteToLog(char*);
void SrvMain(int argc, char** argv);
void CtrlHandler(DWORD request);

int _tmain(int argc, _TCHAR* argv[])
{
	SERVICE_TABLE_ENTRY SrvTbl[] = 
	{
	    { TEXT("SrvMonitor_id"), (LPSERVICE_MAIN_FUNCTION)SrvMain },
		{ NULL, NULL },
	};

	StartServiceCtrlDispatcher(SrvTbl);
	
	return 0;
}

void SrvMain(int argc,char** argv)
{
	gSrvStatus.dwServiceType      = SERVICE_WIN32;
	gSrvStatus.dwCurrentState     = SERVICE_START_PENDING;
	gSrvStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;//只接受系统关机和停止服务两种控制命令
	gSrvStatus.dwWin32ExitCode    = 0;
	gSrvStatus.dwCheckPoint       = 0;
	gSrvStatus.dwWaitHint         = 0;
	gSrvStatus.dwServiceSpecificExitCode = 0;

	ghStatus = RegisterServiceCtrlHandler(TEXT("SrvMonitor_status_id"), (LPHANDLER_FUNCTION)CtrlHandler);//调用RegisterServiceCtrlHandler函数去通知SCM它的CtrlHandler回调函数的地址
	if (ghStatus == 0)
	{
		WriteToLog("RegisterServiceCtrlHandler failed");
		return; 
	}
	
	gSrvStatus.dwCurrentState=SERVICE_RUNNING; //向SCM报告运行状态
	SetServiceStatus(ghStatus, &gSrvStatus);

	STARTUPINFO si;
	PROCESS_INFORMATION pi; //进程信息：
	ZeroMemory(&si, sizeof(si)); 
	si.cb = sizeof(si); 
	ZeroMemory(&pi, sizeof(pi)); 
	do{
		if(!CreateProcess(TEXT(EXE_FILE_NAME), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			WriteToLog("Monitoring stopped. create process failed");
			return; 
		} 
		WaitForSingleObject(pi.hProcess, INFINITE);//检测进程是否停止 
		CloseHandle(pi.hProcess); 
		CloseHandle(pi.hThread);
	} while (gSrvStatus.dwCurrentState == SERVICE_RUNNING);//如果进程推出就再次执行方法 }

	gSrvStatus.dwCurrentState  = SERVICE_STOPPED;
	gSrvStatus.dwWin32ExitCode = -1;
	SetServiceStatus(ghStatus, &gSrvStatus);
	return;
}

void CtrlHandler(DWORD request)
{
	switch(request)
	{
	case SERVICE_CONTROL_STOP:
		gSrvStatus.dwWin32ExitCode = 0;
		gSrvStatus.dwCurrentState  = SERVICE_STOPPED;
		WriteToLog("Monitoring stopped.");
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		gSrvStatus.dwWin32ExitCode = 0;
		gSrvStatus.dwCurrentState  = SERVICE_STOPPED;
		WriteToLog("Monitoring stopped.");
		break;
	default:
		break;
	}
	SetServiceStatus(ghStatus, &gSrvStatus);

	return;
}

int WriteToLog(char* str)
{
	/*
	static FILE* logfp = NULL;
	if (!logfp)
		logfp = fopen(LOGFILE,"w+");
	if (logfp == NULL)
		return -1;
	fprintf(logfp,"%s\n",str);
	fflush(logfp);
	//fclose(logfp);*/
	return 0;
}
#endif
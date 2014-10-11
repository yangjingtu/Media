// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "Encoder.h"
#define AUDIO_SDK extern "C" __declspec(dllexport)
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

HAVENCODER WINAPI FS_Create_Encoder()
{
	return (HAVENCODER)(new CEncoder());
}

void WINAPI FS_Release_Encoder(HAVENCODER hD)
{
	assert(hD != NULL);
	delete (CEncoder*)hD;
}

bool WINAPI FS_Register_Encoder(HAVENCODER hD, int type)
{
	assert(hD != NULL);
	return ((CEncoder*)hD)->SetEncoder(type);
}

void WINAPI FS_UnRegister_Encoder(HAVENCODER hD, int type)
{
	assert(hD != NULL);
	((CEncoder*)hD)->UnRegisterEncoder(type);
}

char* WINAPI FS_GetAUData(HAVENCODER hD, char* data, int lenght, int& newLength)
{
	assert(hD != NULL);
	CEncoder* d = (CEncoder*)hD;
	return d->GetAUBuffer(data, lenght, newLength);
}
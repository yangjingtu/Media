// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "Decoder.h"
#define AUDIO_SDK extern "C" __declspec(dllexport)
#define AUDIO_SDK extern "C" __declspec(dllexport)

CRITICAL_SECTION g_cri;

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			InitializeCriticalSection(&g_cri);
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			DeleteCriticalSection(&g_cri);
		}
		break;
	}
	return TRUE;
}

HAVDECODER WINAPI FS_Create_Decoder()
{
	return (HAVDECODER)(new CDecoder());
}

void WINAPI FS_Release_Decoder(HAVDECODER hD)
{
	assert(hD != NULL);
	delete (CDecoder*)hD;
}

bool WINAPI FS_Register_Decoder(HAVDECODER hD, int type, FSDECORD_CB cb, void* userdata)
{
	assert(hD != NULL);
	return ((CDecoder*)hD)->SetDecoder(type, cb, userdata);
}

bool WINAPI FS_Set_Resolution(HAVDECODER hD, int nWidth, int nHeight)
{
	assert(hD != NULL);
	CDecoder* pDec = (CDecoder*)(hD);
	return pDec->SetResolution(nWidth, nHeight);
}

bool WINAPI FS_Reset_Decoder(HAVDECODER hD, int nWidth, int nHeight)
{
	assert(hD != NULL);
	CDecoder* pDec = (CDecoder*)(hD);
	if(nWidth == 0 && nHeight == 0)
	{
		return pDec->ResetDecoder();
	}
	else
	{
		return pDec->SetResolution(nWidth, nHeight);
	}
	return true;
}

void WINAPI FS_UnRegister_Decoder(HAVDECODER hD, int type)
{
	assert(hD != NULL);
	((CDecoder*)hD)->UnRegisterDecoder(type);
}

char* WINAPI FS_GetYUV420Data(HAVDECODER hD, char* data, int lenght)
{
	assert(hD != NULL);
	CDecoder* d = (CDecoder*)hD;
	return d->GetYUV420Buffer(data, lenght);
}

char* WINAPI FS_GetPCMData(HAVDECODER hD, char* data, int lenght, int& newLength)
{
	assert(hD != NULL);
	CDecoder* d = (CDecoder*)hD;
	return d->GetPCMBuffer(data, lenght, newLength);
}
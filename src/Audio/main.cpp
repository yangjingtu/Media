#include "stdafx.h"
#include "WaveOut.h"
#include "WavePlay.h"
#include "WaveIn.h"
#include "Audio.h"
#include <stdio.h>
#define AUDIO_SDK extern "C" __declspec(dllexport)

HANDLE				g_hDllModule =	NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hDllModule = hModule;
		//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);//REALTIME_PRIORITY_CLASS
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		g_hDllModule = NULL;
		break;
	}
	return TRUE;
}

/**************WaveOut****************************/
AUDIO_SDK HAUDIOOUT WINAPI CreatePlayDev(){
	return (HAUDIOOUT)(new CWaveOut());
}
AUDIO_SDK HAUDIOOUT WINAPI CreatePlayDev2(){
	return (HAUDIOOUT)(new CWavePlay());
}

AUDIO_SDK BOOL WINAPI OpenPlayDev(HAUDIOOUT WaveOut){
	assert(WaveOut != NULL);
	return ((CWaveOut*)WaveOut)->OpenDev();
}

AUDIO_SDK BOOL WINAPI OpenPlayDev2(HAUDIOOUT WaveOut, int nSamples, int nChannels, int nBitSample){
	assert(WaveOut != NULL);
	return ((CWavePlay*)WaveOut)->OpenDev(nSamples, nChannels, nBitSample);
}

AUDIO_SDK BOOL WINAPI SetPlayParam(HAUDIOOUT WaveOut, PARAMTYPE type, LPVOID pData ){
	assert(WaveOut != NULL);
	return ((CWaveOut*)WaveOut)->SetPlayParam(type, pData);
}

AUDIO_SDK BOOL WINAPI WriteData(HAUDIOOUT WaveOut, unsigned char* buf, int size){
	assert(WaveOut != NULL);
	return ((CWaveOut*)WaveOut)->PlayData(buf, size);
}

AUDIO_SDK BOOL WINAPI WriteData2(HAUDIOOUT WaveOut, unsigned char* buf, int size){
	assert(WaveOut != NULL);
	return ((CWavePlay*)WaveOut)->PlayData(buf, size);
}

AUDIO_SDK BOOL WINAPI ClosePlayDev(HAUDIOOUT WaveOut){
	assert(WaveOut != NULL);
	CWaveOut* waveOut = (CWaveOut*)WaveOut;
	int ret = waveOut->CloseDev();
	delete waveOut;
	return ret;
}

AUDIO_SDK BOOL WINAPI ClosePlayDev2(HAUDIOOUT WaveOut){
	assert(WaveOut != NULL);
	CWavePlay* waveOut = (CWavePlay*)WaveOut;
	int ret = waveOut->CloseDev();
	delete waveOut;
	return ret;
}

AUDIO_SDK void WINAPI PausePlay(HAUDIOOUT WaveOut){
	assert(WaveOut != NULL);
}

AUDIO_SDK void WINAPI RestartPlay(HAUDIOOUT WaveOut){
	assert(WaveOut != NULL);
}

AUDIO_SDK void WINAPI ReleasPlayDev(HAUDIOOUT WaveOut){
	assert(WaveOut != NULL);
	delete ((CWaveOut*)WaveOut);
}

/***************WaveIn************************/
AUDIO_SDK void WINAPI SetRecDataCallback(HAUDIOIN hWaveIn, RECDATA fun, LPVOID pUser)
{
	assert(hWaveIn != NULL);
	((CWaveIn*)hWaveIn)->SetCallback(fun, pUser);
}

AUDIO_SDK HAUDIOIN WINAPI CreateRecDev(){
	return (HAUDIOIN)(new CWaveIn());
}

AUDIO_SDK BOOL WINAPI OpenRecDev(HAUDIOIN hWaveIn){
	assert(hWaveIn != NULL);
	return ((CWaveIn*)hWaveIn)->OpenRec();
}

AUDIO_SDK BOOL WINAPI SetRecParam(HAUDIOIN hWaveIn, PARAMTYPE type, LPVOID pData ){
	assert(hWaveIn != NULL);
	return ((CWaveIn*)hWaveIn)->SetRecParam(type, pData);
}

AUDIO_SDK BOOL WINAPI CloseRecDev(HAUDIOIN hWaveIn){
	assert(hWaveIn != NULL);
	CWaveIn* waveIn = (CWaveIn*)hWaveIn;
	int ret = waveIn->CloseDev();
	delete waveIn;
	return ret;
}

AUDIO_SDK BOOL WINAPI StopRecDev(HAUDIOIN hWaveIn){
	assert(hWaveIn != NULL);
	return ((CWaveIn*)hWaveIn)->StopRec();
}

AUDIO_SDK BOOL WINAPI RestartRecDev(HAUDIOIN hWaveIn){
	assert(hWaveIn != NULL);
	return ((CWaveIn*)hWaveIn)->StartRec();
}

AUDIO_SDK void WINAPI ReleasRecDev(HAUDIOIN hWaveIn){
	assert(hWaveIn != NULL);
	delete ((CWaveIn*)hWaveIn);
}
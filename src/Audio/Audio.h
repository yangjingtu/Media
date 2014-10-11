#pragma once
#define HAUDIOOUT	void*
#define HAUDIOIN	void*

#define _EXPORTING

#ifdef	_EXPORTING
#define AUDIO_SDK extern "C" __declspec(dllexport)
#else
#define AUDIO_SDK extern "C" __declspec(dllimport)
#endif

#define WINAPI __stdcall
#define	AUDIO_SDK_VERSION		"1.0.0.0"

/************************************************************************/
enum PARAMTYPE
{
	SETWAVEFORMATEX	= 0x1,
	SETDATAHEADSIZE,
	SETWAVEHDRSIZE
};
/************************************************************************/
/* Audio´¦ÀíÄ£¿é                                                        */
/************************************************************************/

typedef void (*RECDATA)(int type, unsigned char* pData, int length, LPVOID pUser );

/**********************WaveOut***************************/

AUDIO_SDK HAUDIOOUT WINAPI CreatePlayDev();

AUDIO_SDK BOOL WINAPI OpenPlayDev(HAUDIOOUT WaveOut);

AUDIO_SDK BOOL WINAPI SetPlayParam(HAUDIOOUT WaveOut, PARAMTYPE type, LPVOID pData );

AUDIO_SDK BOOL WINAPI WriteData(HAUDIOOUT WaveOut, unsigned char* buf, int size);

AUDIO_SDK BOOL WINAPI ClosePlayDev(HAUDIOOUT WaveOut);

AUDIO_SDK void WINAPI PausePlay(HAUDIOOUT WaveOut);

AUDIO_SDK void WINAPI RestartPlay(HAUDIOOUT WaveOut);

AUDIO_SDK void WINAPI ReleasPlayDev(HAUDIOOUT hWaveIn);


AUDIO_SDK HAUDIOOUT WINAPI CreatePlayDev2();
AUDIO_SDK BOOL WINAPI OpenPlayDev2(HAUDIOOUT WaveOut, int nSamples, int nChannels, int nBitSample);
AUDIO_SDK BOOL WINAPI WriteData2(HAUDIOOUT WaveOut, unsigned char* buf, int size);
AUDIO_SDK BOOL WINAPI ClosePlayDev2(HAUDIOOUT WaveOut);

/***************WaveIn************************/

AUDIO_SDK void WINAPI SetRecDataCallback(HAUDIOIN hWaveIn, RECDATA fun, LPVOID pUser);

AUDIO_SDK HAUDIOIN WINAPI CreateRecDev();

AUDIO_SDK BOOL WINAPI OpenRecDev(HAUDIOIN hWaveIn);

AUDIO_SDK BOOL WINAPI SetRecParam(HAUDIOIN hWaveIn, PARAMTYPE type, LPVOID pData);

AUDIO_SDK BOOL WINAPI CloseRecDev(HAUDIOIN hWaveIn);

AUDIO_SDK BOOL WINAPI StopRecDev(HAUDIOIN hWaveIn);

AUDIO_SDK BOOL WINAPI RestartRecDev(HAUDIOIN hWaveIn);

AUDIO_SDK void WINAPI ReleasRecDev(HAUDIOIN hWaveIn);
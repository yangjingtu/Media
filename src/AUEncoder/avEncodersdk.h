#ifndef	__FSAVENCODERSDK_H
#define __FSAVENCODERSDK_H

#include <iostream>
using namespace std;

#define HAVENCODER	void*

#ifdef	FSAVENCOD_SDK
#else
#define FSAVENCOD_SDK extern "C" __declspec(dllimport)
#endif
#define USE_HSSDK

/*****************************************************************
 * ∫Í∂®“Â
 *****************************************************************/

typedef enum {
	DCD_ENCODE_UNKNOW = 0x100,
	DCD_ENCODE_IMAADPCM,
	DCD_ENCODE_G726,
	DCD_ENCODE_HIG726
} ENCODE_TYPE;

//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" 
{
#endif
	
	FSAVENCOD_SDK HAVENCODER WINAPI FS_Create_Encoder();
	FSAVENCOD_SDK bool WINAPI FS_Register_Encoder(HAVENCODER hD, int type);
	FSAVENCOD_SDK void WINAPI FS_UnRegister_Encoder(HAVENCODER hD, int type);
	FSAVENCOD_SDK void WINAPI FS_Release_Encoder(HAVENCODER hD);
	FSAVENCOD_SDK char* WINAPI FS_GetAUData(HAVENCODER hD, char* data, int lenght, int& newLength);		
#ifdef __cplusplus
}
#endif

#endif
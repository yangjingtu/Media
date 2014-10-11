#ifndef	__FSAVDECODERSDK_H
#define __FSAVDECODERSDK_H
#include <MMSystem.h>
#include <iostream>
using namespace std;

#define HAVDECODER	void*

#define _EXPORTING

#ifdef	_EXPORTING
#define FSAVDECOD_SDK extern "C" __declspec(dllexport)
#else
#define FSAVDECOD_SDK extern "C" __declspec(dllimport)
#endif
#define USE_HSSDK

/*****************************************************************
 * 宏定义
 *****************************************************************/
#define REBOOTDECODER	0
typedef void (*FSDECORD_CB)(int msg, void *param, void *userdata);

typedef enum {
	DCD_UNKNOW = 0x100,
	DCD_IMAADPCM,
	DCD_H264,
	DCD_MJPEG,
	DCD_G726,
	DCD_PCM,
	DCD_HIG726,
	DCD_AAC
} DECODE_TYPE;

/************************************************************************/
/* 结构体                                                               */
/************************************************************************/
typedef struct {
	int			type;
	INT64	ts;
	int	sec;
	int			width;
	int			height;
	int			length;
	char		*frame;
} FRAME_PER, *LPFRAME_P;

//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" 
{
#endif
	
	FSAVDECOD_SDK HAVDECODER WINAPI FS_Create_Decoder();
	FSAVDECOD_SDK bool WINAPI FS_Register_Decoder(HAVDECODER hD, int type, FSDECORD_CB cb, void* userdata);
	FSAVDECOD_SDK bool WINAPI FS_Set_Resolution(HAVDECODER hD,  int nWidth, int nHeight);
	FSAVDECOD_SDK bool WINAPI FS_Reset_Decoder(HAVDECODER hD, int nWidth = 0, int nHeight = 0);
	FSAVDECOD_SDK void WINAPI FS_UnRegister_Decoder(HAVDECODER hD, int type);
	FSAVDECOD_SDK void WINAPI FS_Release_Decoder(HAVDECODER hD);
	FSAVDECOD_SDK char* WINAPI FS_GetYUV420Data(HAVDECODER hD, char* data, int lenght);
	FSAVDECOD_SDK char* WINAPI FS_GetPCMData(HAVDECODER hD, char* data, int lenght, int& newLength);
#ifdef __cplusplus
}
#endif

#endif
#ifndef _COMM_DATA_DEF_H_
#define _COMM_DATA_DEF_H_

#include "../../Include/inttypes.h"

// #ifdef _DEBUG 
// #include <tchar.h>
// void XdPrint(_TCHAR* format, ...) 
// { 
// 	va_list args; 
// 	va_start(args, format); 
// 	_TCHAR buf[256]; 
// 	_vstprintf(buf, format, args); 
// 	OutputDebugString(buf); 
// 	va_end(args); 
// } 
// #else 
// void XdPrint(_TCHAR* format, ...){} 
// #endif 
//////////////////////////////////////////////////////////////////////////
//define
//////////////////////////////////////////////////////////////////////////
#define MAXCHANNEL	9
#define TIMERPOLLING  1


//事件回调
#define WM_EVENT_CALLBACK		WM_USER + 1000	

extern HWND g_hwndPluginCtrl;
#define CB_EVENT(wparam, lparam) ::PostMessage(g_hwndPluginCtrl, WM_EVENT_CALLBACK, (WPARAM)(wparam), (LPARAM)(lparam))

//////////////////////////////////////////////////////////////////////////
//const
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//enum
//////////////////////////////////////////////////////////////////////////
enum EM_MEDIA_TYPE
{
	EM_MEDIA_UNKNOW = 0,
	EM_VIDEO,
	EM_AUDIO,
	EM_AV
};

enum EM_MSG
{
	NET_MSG_CONNECT = 0,
	NET_MSG_VIDEO_OPEN,
	NET_MSG_AUDIO_OPEN
};

typedef enum {
	MF_UNKNOW = 0x100,
	MF_YUYV422,
	MF_YUV420P,
	MF_PCM,
	MF_IMAADPCM,
	MF_H264,
	MF_MJPEG,
	MF_G726,

	MF_VIDEO,
	MF_AUDIO
} MEDIA_FORMAT_T;

typedef enum _EM_EVENT_TYPE
{
	MSG_TWOWND_SWAP = 0,					//两个窗口对调
}EM_EVENT_TYPE;

//////////////////////////////////////////////////////////////////////////
//struct
//////////////////////////////////////////////////////////////////////////
typedef struct {
	int			type;
	int			width;
	int			height;
	long long	timeStamp;
	uint32_t    videoFrameRate;
	uint32_t	length;
	uint32_t	isKeyFrame;
	uint32_t	videoBitRate;
	uint64_t	reserve;
	unsigned char*	frame;
} FRAME_HEAD, *LPFRAME_HEAD;

typedef struct _TwoWndSwapInfo
{
	long oldWnd;
	long newWnd;
}TwoWndSwapInfo, *PTwoWndSwapInfo;

#endif
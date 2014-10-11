/***********************************************************************************************************************
Copyright:  
FileName: ProtoclsFY.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef ADAPTER_PROTOCLSFY_H_
#define ADAPTER_PROTOCLSFY_H_
#include "../Common/Common.h"
#include "../Common/Frame.h"

#define MSGT_MAGIC            *(U32 *)"FYMT"
#define PROTOCL_VER_1_0_0_0   0x10000
#define PROTOCL_ENCRYPT_NULL  0x00        
//Signaling
enum MsgType_
{
	MSGT_REQ_LOGIN = 10000,
	MSGT_RSP_LOGIN,
	MSGT_REQ_LOGOUT,

	MSGT_REQ_STREAM_OPEN,
	MSGT_RSP_STREAM_OPEN,
	MSGT_REQ_STREAM_CLOSE,
	MSGT_RSP_STREAM_CLOSE,

	MSGT_RSP_HEART_BEAT,

	MSGT_REQ_STREAM_START,
	MSGT_RSP_STREAM_START,
	MSGT_REQ_STREAM_STOP,
	MSGT_RSP_STREAM_STOP,

	MSGT_REQ_STREAM_SEEK,
	MSGT_RSP_STREAM_SEEK,
	MSGT_REQ_STREAM_MODE,
	MSGT_RSP_STREAM_MODE,
	MSGT_AV_STREAM_FRAME,
	MSGT_AV_STREAM_EOF,
}MsgType_e;

typedef struct MsgHead_
{
	U32 Magic;
	U16 Ver;
	U16 Encrypt;
	U32 MsgT;
	U32 Len;
	S32 RspId;
	U8  Rv[8];
	S8  Data[0];
}ATTRIBUTE_PACKED MsgHead_t;

typedef struct MsgRspHeart_
{
	U32 Token;
	U8  Rv[8];
}ATTRIBUTE_PACKED MsgRspHeart_t;

typedef struct MsgReqLogin_
{
	S8 UsrName[32];
	S8 Passwd[32];
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgReqLogin_t;

typedef struct MsgRspLogin_
{
	S32 rVal; //0:succ; other:error no
	U64 Token;
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgRspLogin_t;

typedef struct MsgReqLogout_
{
	U64 Token;
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgReqLogout_t;

typedef struct MsgRspLogout_
{
	S32 rVal; //0:succ; other:error no
	U64 Token;
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgRspLogout_t;

typedef struct MsgReqStreamOpen_
{
	S8  URL[256];
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgReqStreamOpen_t;

typedef struct MsgRsStreamOpen_
{
	S32 rVal; //0:succ; other:error no
	U64 Token;
	S32 Frames;
	S32 FrameIntValmSec;
	U16 vWidth;
	U16 vHeight;
	U32 vFrameRate;
	U32 vBitRate;
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgRspStreamOpen_t;

typedef struct MsgReqStreamClose_
{
	U64 Token;
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgReqStreamClose_t;

typedef struct MsgRspStreamClose_
{
	S32 rVal; //0:succ; other:error no
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgRspStreamClose_t;

typedef struct MsgReqStreamStart_
{
	U64 Token;
	U32 BitmapT;  
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgReqStreamStart_t;

typedef struct MsgRspStreamStart_
{
	S32 rVal; //0:succ; other:error no
	S32 ResV; //
	// file info;
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgRspStreamStart_t;

typedef struct MsgReqStreamStop_
{
	U64 Token;
	U32 BitmapT;
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgReqStreamStop_t;

typedef struct MsgRspStreamStop_
{
	S32 rVal; //0:succ; other:error no
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgRspStreamStop_t;

typedef struct MsgReqStreamSeek_
{
	U32 Offset;                          //
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgReqStreamSeek_t;

typedef struct MsgRspStreamSeek_
{
	S32 rVal; //< 0 failed; > 0 CurFrameNo  
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgRspStreamSeek_t;

typedef struct MsgReqStreamMode_
{
	U32 Mode;  //0:all 1: KeyFrame;
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgReqStreamMode_t;

typedef struct MsgRspStreamMode_
{
	S32 rVal; //0:succ; other:error no
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgRspStreamMode_t;

typedef struct MsgAvStreamEof_
{
	U64 Token;
	U8  Rv[64];
}ATTRIBUTE_PACKED MsgAvStreamEof_t;

#endif /* ADAPTER_PROTOCLSFY_H_ */


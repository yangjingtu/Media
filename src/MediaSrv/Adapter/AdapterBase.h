/***********************************************************************************************************************
Copyright:  
FileName: AdapterBase.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/28/8   1.0.0.0     build this file 
************************************************************************************************************************/

#ifndef ADAPTER_ADAPTERBASE_H_
#define ADAPTER_ADAPTERBASE_H_
#include <string>
#include "../Common/Common.h"
using namespace std;

#define ADP                     CAdapterBase
#define ADP_MSG_MAGIC           *(U32 *)"MSG_"

class CAdapterBase
{
public:
	typedef enum AdpCmd_
	{		
		ADP_CMD_REQ_LOGIN = 0,
		ADP_CMD_RSP_LOGIN,
		ADP_CMD_REQ_LOGOUT,

		ADP_CMD_REQ_STREAM_OPEN,
		ADP_CMD_RSP_STREAM_OPEN,
		ADP_CMD_REQ_STREAM_CLOSE,
		ADP_CMD_RSP_STREAM_CLOSE,

		ADP_CMD_RSP_HEART_BEAT,

		ADP_CMD_REQ_STREAM_START,
		ADP_CMD_RSP_STREAM_START,
		ADP_CMD_REQ_STREAM_STOP,
		ADP_CMD_RSP_STREAM_STOP,

		ADP_CMD_REQ_STREAM_SEEK,
		ADP_CMD_RSP_STREAM_SEEK,
		ADP_CMD_REQ_STREAM_MODE,
		ADP_CMD_RSP_STREAM_MODE,
		// iner
		ADP_CMD_AV_FRAME,
		ADP_CMD_AV_EOF,                                           //use AvStreamEof_t

		/***************************************************************************************************************/
		ADP_MSG_NET_STOP,                                         //use rVal;

		ADP_CMD_UNKNOW,
	} AdpCmd_e;

	typedef struct RspHeart_
	{
		U32 Token;
	} RspHeart_t;

	typedef struct ReqLogin_
	{
		S8 UsrName[32];
		S8 Passwd[32];
	} ReqLogin_t;

	typedef struct RspLogin_
	{
		S32 rVal; //0:succ; other error no
		U64 Token;
	} RspLogin_t;

	typedef struct ReqLogout_
	{
		U64 Token;
		U8  Rv[64];
	} ReqLogout_t;

	typedef struct RspLogout_
	{
		S32 rVal; //0:succ; other error no
	} RspLogout_t;

	typedef struct ReqStreamOpen_
	{
		S8  URL[256];
	} ReqStreamOpen_t;

	typedef struct RspStreamOpen_
	{
		S32 rVal;   //0:succ; other:error no
		U64 Token;
		S32 Frames; //total frames          
		S32 FrameIntValmSec;
		U16 vWidth;
		U16 vHeight;
		U32 vFrameRate;
		U32 vBitRate;
	} RspStreamOpen_t;

	typedef struct ReqStreamClose_
	{
		U64 Token;
	} ReqStreamClose_t;

	typedef struct RspStreamClose_
	{
		S32 rVal; //0:succ; other:error no
	} RspStreamClose_t;

	typedef struct ReqStreamStart_
	{
		U64 Token;
		U32 BitMapT;  
	} ReqStreamStart_t;

	typedef struct RspStreamStart_
	{
		S32 rVal; //0:succ; other error no
		S32 ResV; //
		// file info;
	} RspStreamStart_t;

	typedef struct ReqStreamStop_
	{
		U64 Token;
		U32 BitmapT;
	} ReqStreamStop_t;

	typedef struct RspStreamStop_
	{
		S32 rVal; //0:succ; other error no
	} RspStreamStop_t;

	typedef struct ReqStreamSeek_
	{
		U32 Offset;                          //
		// file info;
	} ReqStreamSeek_t;

	typedef struct RspStreamSeek_
	{
		S32 rVal; //< 0 failed; > 0 CurFrameNo  
	} RspStreamSeek_t;

	typedef struct ReqStreamMode_
	{
		U32 Mode;  //0:all 1: KeyFrame;
	} ReqStreamMode_t;

	typedef struct RspStreamMode_
	{
		S32 rVal; //0:succ; other:error no
	} RspStreamMode_t;

	typedef struct AvStreamEof_
	{
		U64 Token;
	} AvStreamEof_t;

	typedef struct ID_
	{
		S32 Id0;
		S32 Id1;
		S32 Id2;
	} ID_t;

	typedef struct Msg_ 
	{
		U32      Magic;
		AdpCmd_e Type;
		U32      Live;
		S32      Id;
		U32      RspId;
		union Body_ 
		{
			S32               rVal;
		    ID_t              ID;
		    RspHeart_t        RspHeart;
		    ReqLogin_t        ReqLogin;
		    RspLogin_t        RspLogin;
		    ReqLogout_t       ReqLogout;
		    ReqStreamStart_t  ReqStreamStart;
		    RspStreamStart_t  RspStreamStart;
			ReqStreamStop_t   ReqStreamStop;
			RspStreamStop_t   RspStreamStop;
			ReqStreamOpen_t   ReqStreamOpen;
			RspStreamOpen_t   RspStreamOpen;
			ReqStreamClose_t  ReqStreamClose;
			RspStreamClose_t  RspStreamClose;
		    ReqStreamSeek_t   ReqStreamSeek;
			RspStreamSeek_t   RspStreamSeek;
			ReqStreamMode_t   ReqStreamMode;
			RspStreamMode_t   RspStreamMode;
			AvStreamEof_t     AvStreamEof;
			Frame_t*          pFrame;
			Void*             pObj;
		} Body;
	} Msg_t;

public:
	virtual S32  Fill(Msg_t& Msg, S8* pBuf, U32 Len) = 0; // succ return adpter length else return -1;
	virtual Bool Parser(Msg_t& Msg) = 0;

public:
	CAdapterBase() { }
	virtual ~CAdapterBase() { }
};

#endif /* ADAPTER_ADAPTERBASE_H_ */

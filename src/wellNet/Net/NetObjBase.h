/***********************************************************************************************************************
Copyright:  
FileName: NetObjBase.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef NET_NETOBJBASE_H_
#define NET_NETOBJBASE_H_
#include "../Adapter/AdapterBase.h"
#include "../Common/Trash.h"

#define NET_TIMEOUT_MSEC             (20*1000)

typedef S32 (* pAvFrameFunc)(Frame_t *pFrame, Void *Param);
typedef S32 (* pMsgFunc)(const ADP::Msg_t& msg, Void *Param);

typedef struct InitObj_
{
	pMsgFunc       MsgCb;
	Void*          pMsgCbParam;
	pAvFrameFunc   AvFrameCb;
	Void*          pAvFrameCbParam;
}InitObjInfo_t;

class WELL_SDK CNetOBjBase// : public CTrash
{
public:
	virtual ~CNetOBjBase(){}

	virtual ADP::RspStreamOpen_t  Open(ADP::ReqStreamOpen_t Req, InitObjInfo_t Info, S32 mSec = NET_TIMEOUT_MSEC) = 0;
	virtual ADP::RspStreamClose_t Close(ADP::ReqStreamClose_t Req, S32 mSec = NET_TIMEOUT_MSEC) = 0;
	virtual ADP::RspLogin_t       Login(ADP::ReqLogin_t Req, S32 mSec = NET_TIMEOUT_MSEC) = 0;
	virtual ADP::RspLogout_t      Logout(ADP::ReqLogout_t Req, S32 mSec = NET_TIMEOUT_MSEC) = 0;
	virtual ADP::RspStreamStart_t StreamStart(ADP::ReqStreamStart_t Req, S32 mSec = NET_TIMEOUT_MSEC) = 0;
	virtual ADP::RspStreamStop_t  StreamStop(ADP::ReqStreamStop_t Req, S32 mSec = NET_TIMEOUT_MSEC) = 0;
	virtual ADP::RspStreamSeek_t  SeekStream(ADP::ReqStreamSeek_t Req, S32 mSec = NET_TIMEOUT_MSEC) = 0;
	virtual ADP::RspStreamMode_t  SetStreamMode(ADP::ReqStreamMode_t Req, S32 mSec = NET_TIMEOUT_MSEC) = 0;

public:
	virtual S32 Run() = 0;
};

#endif /*NET_NETOBJBASE_H_*/
/***********************************************************************************************************************
Copyright:  
FileName: NetObj.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef NET_NETOBJ_H_
#define NET_NETOBJ_H_
#include <string>
#include <list>
#include "../Common/Common.h"
#include "../Adapter/AdapterFY.h"
#include "../Thread/AutoDoT.hpp"
#include "StreamFile.h"
#include "NetObjBase.h"
#include "RcvBuf.h"
#include "SndBuf.h"
#include "Sock.h"
using namespace std;

class CCrowd;

class WELL_SDK CNetObj: public CNetOBjBase
{
public:
	CNetObj(Socket_t sHandle, CCrowd* pCrowd, S32 SndBufSize, S32 RcvBufSize);//Server
	CNetObj(S32 SndBufSize, S32 RcvBufSize); //Client
	virtual ~CNetObj();

public:
	virtual ADP::RspStreamOpen_t  Open(ADP::ReqStreamOpen_t Req, InitObjInfo_t Info, S32 mSec = NET_TIMEOUT_MSEC);
	virtual ADP::RspStreamClose_t Close(ADP::ReqStreamClose_t Req, S32 mSec = NET_TIMEOUT_MSEC);
	virtual ADP::RspLogin_t       Login(ADP::ReqLogin_t Req, S32 mSec = NET_TIMEOUT_MSEC);
	virtual ADP::RspLogout_t      Logout(ADP::ReqLogout_t Req, S32 mSec = NET_TIMEOUT_MSEC);
	virtual ADP::RspStreamStart_t StreamStart(ADP::ReqStreamStart_t Req, S32 mSec = NET_TIMEOUT_MSEC);
	virtual ADP::RspStreamStop_t  StreamStop(ADP::ReqStreamStop_t Req, S32 mSec = NET_TIMEOUT_MSEC);
	virtual ADP::RspStreamSeek_t  SeekStream(ADP::ReqStreamSeek_t Req, S32 mSec = NET_TIMEOUT_MSEC);
	virtual ADP::RspStreamMode_t  SetStreamMode(ADP::ReqStreamMode_t Req, S32 mSec = NET_TIMEOUT_MSEC);

public:
	virtual S32 Run();

public:
	static tProcType_t NetThreadProc(tProcPara_t Param);

protected:
	S32  SndData(S8 *Buf, S32 Len, S32 Times);
	S32  RcvData(S8 *Buf, S32 Len, S32 Times);
	S32  SndMsg(ADP::Msg_t& Msg, S32 mSec = NET_TIMEOUT_MSEC);
	S32  RcvMsg(ADP::Msg_t& Msg, S32 mSec = NET_TIMEOUT_MSEC);
	S32  PutMsg(ADP::Msg_t& Msg);
	S32  ParseData();
	S32  SndAvStream(S32 Times);
	Void Stop();
	Void ResetSock();
	string GetUrl(string Url);

protected:
	CAdapterBase*                m_pAdapter;     
	CRcvBuf                      m_RcvBuf;
	CSndBuf                      m_SndBuf;
	CSock                        m_Sock;
	CCrowd*                      m_pCrowd;
	Bool                         m_bRuning;
	Bool                         m_bAvStreamSnd;
	Bool                         m_bResetSock;
	Bool                         m_bClient;
	CStreamBase*                 m_pStream;
	CLock                        m_SndMsgLock;
	
	string                       m_StrUrl; //ip:port:url
	InitObjInfo_t                m_ObjInitInfo;
	tHandle_t                    m_tNetHandle;
	U32                          m_MsgRspId;
	U64                          m_Token;
	CMsgMapT<U32, ADP::Msg_t>    m_RcvMsgMap;
	
	Bool						m_bSafeDelete;
};


#endif /*NET_NETOBJ_H_*/
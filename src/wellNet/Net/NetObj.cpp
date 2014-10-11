/***********************************************************************************************************************
Copyright:  
FileName: NetObj.cpp  
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#include "stdafx.h"
#include "NetObj.h"


CNetObj::CNetObj(Socket_t sHandle, CCrowd *pCrowd, S32 SndBufSize, S32 RcvBufSize)
:m_Sock(sHandle)
{ 
	m_ObjInitInfo.pMsgCbParam     = NULL;
	m_ObjInitInfo.pAvFrameCbParam = NULL;
	m_ObjInitInfo.MsgCb           = NULL;
	m_ObjInitInfo.AvFrameCb       = NULL;
	m_SndBuf.Init(SndBufSize);
	m_RcvBuf.Init(64, RcvBufSize);
	m_pCrowd       = pCrowd;
	m_bRuning      = false;
	m_bClient      = false;
	m_bAvStreamSnd = false;
	m_bResetSock   = false;
	m_MsgRspId     = 0;
	m_pAdapter     = new CAdapterFY(m_RcvBuf);
	m_pStream      = new CStreamFile;

	m_bSafeDelete = false;
}

CNetObj::CNetObj(S32 SndBufSize, S32 RcvBufSize)
{
	m_ObjInitInfo.pMsgCbParam     = NULL;
	m_ObjInitInfo.pAvFrameCbParam = NULL;
	m_ObjInitInfo.MsgCb           = NULL;
	m_ObjInitInfo.AvFrameCb       = NULL;
	m_SndBuf.Init(SndBufSize);
	m_RcvBuf.Init(64, RcvBufSize);
	m_pCrowd       = NULL;
	m_bRuning      = false;
	m_bClient      = true;
	m_bAvStreamSnd = false;
	m_bResetSock   = false;
	m_MsgRspId     = 0;
	m_pAdapter     = new CAdapterFY(m_RcvBuf);
	m_pStream      = new CStreamFile;

	m_bSafeDelete = false;
}

CNetObj::~CNetObj()
{
	m_bRuning = false;
	if (m_pStream)
	{
		delete m_pStream;
		m_pStream = NULL;
	}
	if (m_pAdapter)
	{
		delete m_pAdapter;
		m_pAdapter = NULL;
	}
	m_RcvBuf.UnInit();
	m_SndBuf.UnInit();
}

ADP::RspStreamOpen_t CNetObj::Open(ADP::ReqStreamOpen_t Req, InitObjInfo_t Info, S32 mSec)
{
	ADP::RspStreamOpen_t RspVal;

	RspVal.rVal = m_Sock.SetClient(Req.URL);
	if (RspVal.rVal != MW_SUCC)
	{
		DEBUG_ERROR("Connect err\n");
		return RspVal;
	}
	m_ObjInitInfo  = Info;
	m_StrUrl       = Req.URL;
	m_bResetSock   = false;
	m_bRuning      = true;
	m_bSafeDelete = false;
	m_tNetHandle   = ::CreateNormalThread(NetThreadProc, this, 256 * 1024);
	
	ADP::Msg_t Msg;
	Msg.Type = ADP::ADP_CMD_REQ_STREAM_OPEN;
	Msg.Body.ReqStreamOpen = Req; 
	Msg.RspId = m_MsgRspId++;
	RspVal.rVal = SndMsg(Msg, mSec);
	if (RspVal.rVal != MW_SUCC)
	{
		goto ERR_EXIT;
	}

	RspVal.rVal = RcvMsg(Msg, mSec);
	if (RspVal.rVal != MW_SUCC)
	{
		RspVal.rVal = NET_MSG_TIME_OUT;
		goto ERR_EXIT;
	}
	if (MW_SUCC != Msg.Body.RspStreamOpen.rVal)
	{
		Msg.Type = ADP::ADP_CMD_REQ_STREAM_OPEN;
		Msg.Body.ReqStreamClose.Token = Msg.Body.RspStreamOpen.Token;
		SndMsg(Msg, mSec);
		RspVal.rVal = Msg.Body.RspStreamOpen.rVal;
		goto ERR_EXIT;
	}
	
	return Msg.Body.RspStreamOpen;

ERR_EXIT:
	m_Sock.ClearSock();
	Stop();
	return RspVal;
}

ADP::RspStreamClose_t CNetObj::Close(ADP::ReqStreamClose_t Req, S32 mSec)
{
	m_bRuning    = false;
	m_bResetSock = false;

	ADP::RspStreamClose_t RspVal;
	ADP::Msg_t Msg;	
	Msg.Type = ADP::ADP_CMD_REQ_STREAM_CLOSE;
	Msg.Body.ReqStreamClose = Req; 
	Msg.RspId = m_MsgRspId++;
	RspVal.rVal = SndMsg(Msg, mSec);
	if (RspVal.rVal != MW_SUCC)
	{
		DEBUG_ERROR("Snd msg err\n");
	}

	while (true)
	{
		if(m_bSafeDelete)
		{
			break;
		}
		Sleep(5);
	}
	//RspVal.rVal = RcvMsg(Msg, mSec);
	//if (RspVal.rVal != MW_SUCC)
	//{
	//	RspVal.rVal = NET_MSG_TIME_OUT;
	//	return RspVal;
	//}

	return RspVal;
}


ADP::RspLogin_t CNetObj::Login(ADP::ReqLogin_t Req, S32 mSec)
{
	ADP::RspLogin_t RspVal = { 0 };

	return RspVal;
}

ADP::RspLogout_t CNetObj::Logout(ADP::ReqLogout_t Req, S32 mSec)
{
	ADP::RspLogout_t RspVal = { 0 };

	return RspVal;
}

ADP::RspStreamStart_t CNetObj::StreamStart(ADP::ReqStreamStart_t Req, S32 mSec)
{
	ADP::RspStreamStart_t RspVal = { 0 };

	return RspVal;
}

ADP::RspStreamStop_t CNetObj::StreamStop(ADP::ReqStreamStop_t Req, S32 mSec)
{
	ADP::RspStreamStop_t RspVal = { 0 };

	return RspVal;
}

ADP::RspStreamSeek_t CNetObj::SeekStream(ADP::ReqStreamSeek_t Req, S32 mSec)
{
	ADP::RspStreamSeek_t RspVal;
	ADP::Msg_t Msg;	
	Msg.Type = ADP::ADP_CMD_REQ_STREAM_SEEK;
	Msg.Body.ReqStreamSeek = Req; 
	Msg.RspId = m_MsgRspId++;
	RspVal.rVal = SndMsg(Msg, mSec);
	if (RspVal.rVal != MW_SUCC)
	{
		DEBUG_ERROR("Snd msg err\n");
		return RspVal;
	}

	RspVal.rVal = RcvMsg(Msg, mSec);
	if (RspVal.rVal != MW_SUCC)
	{
		RspVal.rVal = NET_MSG_TIME_OUT;
		return RspVal;
	}

	return Msg.Body.RspStreamSeek;
}

ADP::RspStreamMode_t CNetObj::SetStreamMode(ADP::ReqStreamMode_t Req, S32 mSec)
{
	ADP::RspStreamMode_t RspVal;
	ADP::Msg_t Msg;	
	Msg.Type = ADP::ADP_CMD_REQ_STREAM_MODE;
	Msg.Body.ReqStreamMode = Req; 
	Msg.RspId = m_MsgRspId++;
	RspVal.rVal = SndMsg(Msg, mSec);
	if (RspVal.rVal != MW_SUCC)
	{
		DEBUG_ERROR("Snd msg err\n");
		return RspVal;
	}

	RspVal.rVal = RcvMsg(Msg, mSec);
	if (RspVal.rVal != MW_SUCC)
	{
		RspVal.rVal = NET_MSG_TIME_OUT;
		return RspVal;
	}

	return Msg.Body.RspStreamMode;
}

tProcType_t CNetObj::NetThreadProc(tProcPara_t Param)
{
	if (Param) 
	{
		CNetObj *pSlef = (CNetObj *)Param;
		pSlef->Run();
	}

	return NULL;
}

S32 CNetObj::SndData(S8 *Buf, S32 Len, S32 Times)
{
	S32 Timeout = 0, Sent = 0, SndSize = Len;

	while (SndSize > 0 && m_bRuning) 
	{
		Sent = m_Sock.Send(Buf + (Len - SndSize), SndSize, 0);
		if (Sent == SOCK_HANDLE_ERROR)
		{
			//if (m_bClient)
			//{
			//	ResetSock();
			//} 
			//else
			//{
			//	Stop();
			//}
			if (m_ObjInitInfo.MsgCb)
			{
				ADP::Msg_t Msg;
				Msg.Magic     = ADP_MSG_MAGIC;
				Msg.Type      = ADP::ADP_MSG_NET_STOP;
				Msg.Live      = GET_TICKS();
				Msg.Body.rVal = Sent;
				m_ObjInitInfo.MsgCb(Msg, m_ObjInitInfo.pMsgCbParam);
			}
			Stop();
			DEBUG_ERROR("[ %s:%d Sock err:%d ] \n", __FILE__, __LINE__, Sent);
			return MW_FAILED;
		}
		else if (Sent == SOCK_HANDLE_BUFFULL)
		{
			if (Times == 0) // send all
			{
				continue;
			}
			if (Timeout++ > Times) 
			{
				return (Len - SndSize);
			}
			continue;
		}
		Timeout = 0;
		SndSize -= Sent;
	}

	return (Len - SndSize);
}

S32 CNetObj::RcvData(S8 *Buf, S32 Len, S32 Times)
{
	S32 Cnt = 0, Size = SOCK_HANDLE_NODATA, rVal = 0;

	while (Cnt++ < Times && m_bRuning) 
	{
		Size = m_Sock.Recv(Buf, Len, 0);
		if (SOCK_HANDLE_NODATA == Size)
		{
			continue;
		}
		else if (SOCK_HANDLE_CLOSE == Size || SOCK_HANDLE_ERROR == Size)
		{
			//if (m_bClient)
			//{
			//	ResetSock();
			//} 
			//else
			//{
			//	Stop();
			//}
			if (m_ObjInitInfo.MsgCb)
			{
				ADP::Msg_t Msg;
				Msg.Magic     = ADP_MSG_MAGIC;
				Msg.Type      = ADP::ADP_MSG_NET_STOP;
				Msg.Live      = GET_TICKS();
				Msg.Body.rVal = Size;
				m_ObjInitInfo.MsgCb(Msg, m_ObjInitInfo.pMsgCbParam);
			}
			Stop();
			rVal = MW_FAILED;
			DEBUG_ERROR("[ %s:%d Sock err:%d ] \n", __FILE__, __LINE__, Size);
			break;
		}
		else if (Size > 0)
		{
			rVal = Size;
			break;
		}
	}

	return rVal;
}

S32  CNetObj::PutMsg(ADP::Msg_t& Msg)
{
	return m_RcvMsgMap.Put(Msg.RspId, Msg);
}

S32  CNetObj::RcvMsg(ADP::Msg_t& Msg, S32 mSec)
{
	return m_RcvMsgMap.Get(Msg.RspId, Msg, mSec);
}

S32 CNetObj::SndMsg(ADP::Msg_t& Msg, S32 mSec)
{
	CAutoDoT<CLock> Auto(m_SndMsgLock);
	if (m_SndBuf.DataLen() > 0)
	{
		S32 Sent = SndData(m_SndBuf.GetRdPos(), m_SndBuf.DataLen(), 0);
		if (Sent < 0)
		{
			DEBUG_ERROR("%s(err)[ %s(%d)]\n", __FUNCTION__, __FILE__, __LINE__);
		}
		m_SndBuf.Clear();
	}
	
	Msg.Magic = ADP_MSG_MAGIC;
	Msg.Live  = GET_TICKS();
	Msg.Id    = m_SndBuf.FreeLen();

	S32 Size = m_pAdapter->Fill(Msg, m_SndBuf.GetWrPos(), Msg.Id);
	if (Size < 0) {
		DEBUG_ERROR("Snd msg cmd = %d failed, adapter fill", Msg.Type);
		return Size;
	}

	m_SndBuf.IncLen(Size);
	Size = SndData(m_SndBuf.GetRdPos(), m_SndBuf.DataLen(), 300);
	if (Size < 0) 
	{
		m_SndBuf.Clear();
		DEBUG_ERROR("Snd msg cmd = %d failed, senddata", Msg.Type);
		return NET_MSG_SEND_ERR;
	}
	m_SndBuf.DecLen(Size);

	return MW_SUCC;
}


S32 CNetObj::SndAvStream(S32 Times)
{
	ADP::Msg_t Msg;
	memset(&Msg, 0x00, sizeof(ADP::Msg_t));
	Msg.Type      = ADP::ADP_CMD_AV_FRAME;
	Msg.RspId     = m_MsgRspId++; 
	Msg.Body.pObj = m_pStream;
	S32 rVal;
	while (Times-- > 0 && m_bAvStreamSnd && m_bRuning)
	{
		rVal = SndMsg(Msg);
		if (AV_STREAM_END_DATA == rVal)
		{
			Msg.Type  = ADP::ADP_CMD_AV_EOF;
			Msg.RspId = m_MsgRspId++;
			Msg.Body.AvStreamEof.Token = m_Token;
			SndMsg(Msg);

			m_bAvStreamSnd = false;
			break;
		}
		if (MW_SUCC != rVal )
		{
			break;
		}
	}

	return Times;
}


S32 CNetObj::ParseData()
{ 
	ADP::Msg_t Msg;  
	BOOL bGoOn   = false;  
	//memset(&Msg, 0x00, sizeof(ADP::Msg_t));
	Msg.Magic = ADP_MSG_MAGIC;
	//DEBUG_INFO("parse socktag = %d func=%s line=%d\n", tag,  __func__,  __LINE__ );
	do 
	{
		bGoOn = m_pAdapter->Parser(Msg);
		switch (Msg.Type) 
		{
		case ADP::ADP_CMD_REQ_LOGIN:
			Msg.Type                = ADP::ADP_CMD_RSP_LOGIN;
			Msg.Body.RspLogin.rVal  = MW_SUCC;
			Msg.Body.RspLogin.Token = GET_TICKS();
			SndMsg(Msg);

			break;
		case ADP::ADP_CMD_RSP_LOGIN:

			break;
		case ADP::ADP_CMD_REQ_LOGOUT:

			break;
		case ADP::ADP_CMD_RSP_HEART_BEAT:
			//
			break;
		case ADP::ADP_CMD_REQ_STREAM_OPEN:
			m_StrUrl = Msg.Body.ReqStreamOpen.URL;
			Msg.Body.RspStreamOpen.rVal = m_pStream->Open(GetUrl(m_StrUrl).c_str(), AV_STREAM_MODE);
			if ( MW_SUCC != Msg.Body.RspStreamOpen.rVal )
			{
				m_pStream->Close();
				Msg.Body.RspStreamOpen.rVal = NET_MSG_OPEN_FILE_ERR;
			} 
			else
			{
				m_bAvStreamSnd  = true;
//				AudioHead_t a;
				VideoHead_t v;
//				m_pStream->GetAHead(a);
				m_pStream->GetVHead(v);
//				Msg.Body.RspStreamOpen.aSampleRate     = a.SampleRate;
//				Msg.Body.RspStreamOpen.aBitsPerSample  = a.BitsPerSample;
				Msg.Body.RspStreamOpen.vWidth          = v.VWidth;
				Msg.Body.RspStreamOpen.vHeight         = v.VHeight;
				Msg.Body.RspStreamOpen.vBitRate        = v.VBitRate;
				Msg.Body.RspStreamOpen.vFrameRate      = v.VFrameRate;
				Msg.Body.RspStreamOpen.Token           = (m_Token = GET_TICKS());
				Msg.Body.RspStreamOpen.Frames          = m_pStream->GetFrames();
				Msg.Body.RspStreamOpen.FrameIntValmSec = m_pStream->GetIntVal(); 
			}
			Msg.Type = ADP::ADP_CMD_RSP_STREAM_OPEN;
			SndMsg(Msg);

			break;
		case ADP::ADP_CMD_REQ_STREAM_CLOSE:
			m_bAvStreamSnd  = false;
			//Msg.Type = ADP::ADP_CMD_RSP_STREAM_CLOSE;
			//Msg.Body.RspStreamClose.rVal = MW_SUCC;
			//SndMsg(Msg);
			m_pStream->Close();
			Stop();
			break;
		case ADP::ADP_CMD_REQ_STREAM_SEEK:
			Msg.Body.RspStreamSeek.rVal = m_pStream->Seek(Msg.Body.ReqStreamSeek.Offset);
			Msg.Type = ADP::ADP_CMD_RSP_STREAM_SEEK;
			SndMsg(Msg);

			break;
		case ADP::ADP_CMD_REQ_STREAM_MODE:
			Msg.Body.RspStreamMode.rVal = m_pStream->SetRdMode(Msg.Body.ReqStreamMode.Mode); 
			Msg.Type = ADP::ADP_CMD_RSP_STREAM_MODE;
			SndMsg(Msg);

			break;
		case ADP::ADP_CMD_AV_FRAME:
			if (m_ObjInitInfo.AvFrameCb)
			{
				m_ObjInitInfo.AvFrameCb(Msg.Body.pFrame, m_ObjInitInfo.pAvFrameCbParam);
			}

			break;
		case ADP::ADP_CMD_AV_EOF:
			if (m_ObjInitInfo.MsgCb)
			{
				m_ObjInitInfo.MsgCb(Msg, m_ObjInitInfo.pMsgCbParam);
			}

			break;
		case ADP::ADP_CMD_RSP_STREAM_OPEN:
		case ADP::ADP_CMD_RSP_STREAM_CLOSE:
		case ADP::ADP_CMD_RSP_STREAM_SEEK:
		case ADP::ADP_CMD_RSP_STREAM_MODE:
			PutMsg(Msg);

			break;
		default:

			break;
		}
		m_RcvBuf.Update();
	} while (bGoOn && m_bRuning);

	return MW_SUCC;
}

Void CNetObj::Stop()
{
	DEBUG_ERROR("[ %s:%d Stop running ] \n", __FILE__, __LINE__);
	m_bRuning = false;
}

Void CNetObj::ResetSock()
{
	//DEBUG_ERROR("[ %s:%d ResetSock Sock ] \n", __FILE__, __LINE__);
	m_bResetSock = true;
	m_bSafeDelete = false;
}

string CNetObj::GetUrl(string Url)
{
	S8  Ip[20]   = { 0 };
	S32 Port     =   0;
	S8  URL[256] = { 0 };
	sscanf_s(Url.c_str(), "ip=%[^&]&port=%d&url=%s", Ip, sizeof(Ip), &Port, URL, sizeof(256));

	return URL;
}

S32 CNetObj::Run()
{
	DEBUG_INFO("CNetObj::Run()...\n");
	//Test;
	//m_pStream->Open("E:\\Src\\Video\\TestVideo.avi", AV_STREAM_MODE);
	//m_bAvStreamSnd  = true;

	S32 rSize   = 0;
	S32 FreeLen = 0;

	timeval tv = { 0, 200 * 1000 };
	fd_set rFds;
	fd_set wFds;
	fd_set * pWFds = NULL;

	m_bRuning = true;
	while (m_bRuning) 
	{	
		if (m_bResetSock)
		{
			if (MW_SUCC != m_Sock.Reconnect())
			{
				sleep(3);
				continue;
			}
			m_bResetSock = false;
		}
		FD_ZERO(&rFds);
		FD_ZERO(&wFds);
		FD_SET(m_Sock.Handle(), &rFds);
		FD_SET(m_Sock.Handle(), &wFds);
		if (m_bAvStreamSnd)
		{
			pWFds = &wFds;
		}
		else
		{
			pWFds = NULL;
		} 
		if (::select(m_Sock.Handle() + 1, &rFds, pWFds, NULL, &tv) <= 0) 
		{ 
			continue; 
		}
		if (FD_ISSET(m_Sock.Handle(), &rFds)) 
		{
			FreeLen = m_RcvBuf.GetFreeLen();
			if (FreeLen < 64) 
			{
				DEBUG_INFO("The packet is to bigger...\n");
				m_RcvBuf.Clear();
				FreeLen = m_RcvBuf.GetFreeLen();
			}
			rSize = RcvData(m_RcvBuf.GetWrPos(),  FreeLen, 3);
			if (rSize > 0) 
			{
				m_RcvBuf.AddDataLen(rSize);
				ParseData();
			} 
		}

 		if (pWFds && FD_ISSET(m_Sock.Handle(), &wFds)) 
		{
			SndAvStream(5);
		}
	}
	m_bSafeDelete = true;
	//ObjStop();

	DEBUG_INFO("Session exit sHandle:%d\n", m_Sock.Handle());

	return MW_SUCC;
}
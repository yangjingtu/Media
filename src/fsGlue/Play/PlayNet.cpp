#include <WinSock2.h>
#include "PlayNet.h"
#include "../../wellNet/Net/NetObj.h"
#include "../Util/CriLock.h"
#include <sstream>
using namespace std;

void CPlayNet::InitServPort()
{
	m_Ports[0] = 55555;
	m_Ports[1] = 9527;
	m_Ports[2] = 9088;
	m_Ports[3] = 12345;
	m_Ports[4] = 65432;
}

CPlayNet::CPlayNet(CDDrawView* ddraw, short* nWndId)
	: CPlayHelper(ddraw, nWndId)
	, m_pNet(NULL)
	, m_isSkipIFrame(false)
{
	InitServPort();
	Init();
}

CPlayNet::~CPlayNet(void)
{
	UnInit();
}

void CPlayNet::Init()
{
	WRITELOG("CPlayNet::Init");
//	m_pNet = InitNetObj(100 * 1024, 1*1024*1024);
	m_pNet = new CNetObj(100 * 1024, 1 * 1024 * 1024);
	assert(m_pNet != NULL);
}

void CPlayNet::UnInit()
{
	WRITELOG("CPlayNet::UnInit");
	m_isSkipIFrame = false;
	if(m_pNet)
	{
		//m_pNet->Delete();
		delete m_pNet;
		m_pNet = NULL;
	}
//	UnInitNetObj(m_pNet);
}

//////////////////////////////////////////////////////////////////////////
//url="ip=127.0.0.1&port=5555&url=D:\\aaa\bbb\\cc.avi
//////////////////////////////////////////////////////////////////////////
bool CPlayNet::Open(const char* url, short nType)
{
	if(!m_pNet)
		return false;

	WRITELOG("CPlayNet::Open");
	InitObjInfo_t info;
	info.AvFrameCb = &CPlayHelper::frameFunc;
	info.MsgCb = &CPlayHelper::msgCallFunc;
	info.pAvFrameCbParam = this;
	info.pMsgCbParam = this;

	//循环连接
	//如果服务器有一个端口被占用的话，用另外的端口
	S8  Ip[20]   = { 0 };
	S32 Port     =   0;
	S8  Url[256] = { 0 };
	sscanf_s(url, "ip=%[^&]&port=%d&url=%s", Ip, 20, &Port, Url, 256);
	int nIndex = 0;
	stringstream ss;
	do{
		ss.str("");
		ss << "ip=" << Ip
			<< "&port=" << m_Ports[nIndex++]
			<< "&url=" << Url;

		ADP::ReqStreamOpen_t Req;
		memcpy_s(Req.URL, 256, ss.str().c_str(), 256);
		int nTimeOut = 1000 * 3;

		m_NetRlt = m_pNet->Open(Req, info, nTimeOut);
		if(m_NetRlt.rVal == 0)
		{
			break;
		}
	}while (nIndex < 5);

	if(nIndex == 5)
	{
		return false;
	}

//	m_NetRlt = OpenNetObj(m_pNet, info, url);
	SetResolution(m_NetRlt.vWidth, m_NetRlt.vHeight);

	m_isOpen = true;
	m_nTotalFrame = m_NetRlt.Frames;

	return true;
}

bool CPlayNet::Close()
{
	if(m_pNet)
	{
		WRITELOG("CPlayNet::Close");
		ADP::ReqStreamClose_t Req;
		Req.Token = m_NetRlt.Token;
		m_pNet->Close(Req);
//		CloseNetObj(m_pNet, m_NetRlt.Token);
	}
	m_isOpen = false;
	m_isSkipIFrame = false;

	return true;
}

bool CPlayNet::Seek(long pos, long min, long max)
{
	LOCK;
	if(m_pNet)
	{
		S32 totalF = m_NetRlt.Frames;
		S32 offset = ((double)pos / (max - min)) * totalF;

		ADP::ReqStreamSeek_t req;
		req.Offset = offset;
		ADP::RspStreamSeek_t resq = m_pNet->SeekStream(req);
//		bool bSeek = SeekNetObj(m_pNet, offset);
		
		if(m_isSkipIFrame)
		{
			SkipIFrame(false);
		}

		return resq.rVal == 0 ? true : false;
	}
	return false;
}

bool CPlayNet::Forward(short nSpeed)
{
	LOCK;
	SetForwardSpeed(nSpeed);
	if(nSpeed >= 2 && m_pNet)
	{
		if(!m_isSkipIFrame)
		{
			SkipIFrame(true);
		}
	}
	if(nSpeed == 1)
	{
		//1倍速不跳帧
		if(m_isSkipIFrame)
		{
			SkipIFrame(false);
		}
	}

	return true;
}

bool CPlayNet::Reverse(short nSpeed)
{
	SetReverseSpeed(nSpeed);
	if(m_isSkipIFrame)
	{
		SkipIFrame(false);
	}
	return true;
}

bool CPlayNet::NextFrame()
{
	if(m_isSkipIFrame)
	{
		SkipIFrame(false);
	}
	return CPlayHelper::NextFrame();
}

bool CPlayNet::Pause()
{
	if(m_isSkipIFrame)
	{
		SkipIFrame(false);
	}
	return CPlayHelper::Pause();
}

void CPlayNet::SkipIFrame(bool isSkip)
{
	if(m_pNet)
	{
		m_isSkipIFrame = isSkip;
		ADP::ReqStreamMode_t req;
		req.Mode = isSkip ? 1 : 0;			//跳I帧
		ADP::RspStreamMode_t resp = m_pNet->SetStreamMode(req);
	//	SetStreamMode(m_pNet, isSkip ? 1 : 0);
	}
}

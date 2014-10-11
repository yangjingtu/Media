/***********************************************************************************************************************
Copyright:  
FileName: AdapterFY.cpp    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/28/8   1.0.0.0     build this file 
************************************************************************************************************************/
#include "stdafx.h"
#include "AdapterFY.h"
#include "ProtoclsFY.h"
#include "../Net/StreamBase.h"


CAdapterFY::CAdapterFY(CRcvBuf& Buf)
	: m_RcvBuf(Buf)
{
}


CAdapterFY::~CAdapterFY()
{

}


S8 *CAdapterFY::FillMsgHead(U32 CMD, S8 *pBuf, S32& Len, U32 RspId)
{
	MsgHead_t *pMsgHead = (MsgHead_t*)pBuf;
	pMsgHead->Magic   = MSGT_MAGIC;
	pMsgHead->Ver     = PROTOCL_VER_1_0_0_0;
	pMsgHead->Encrypt = PROTOCL_ENCRYPT_NULL;
	pMsgHead->MsgT    = CMD;
	pMsgHead->RspId   = RspId;
	pMsgHead->Len     = Len;
	Len += sizeof(MsgHead_t);
	
	return pMsgHead->Data;

}


S32 CAdapterFY::Fill(ADP::Msg_t& Msg, S8 *pBuf, U32 Len)
{
	S32 rLen  = -1;
	S8* pBody = NULL;
	switch (Msg.Type) 
	{
	case ADP_CMD_REQ_LOGIN:
		break;
	case ADP_CMD_RSP_LOGIN:
		MsgRspLogin_t* pRspLogin;
		rLen      = sizeof(MsgRspLogin_t);
		pRspLogin = (MsgRspLogin_t* )FillMsgHead(MSGT_RSP_LOGIN, pBuf, rLen, Msg.RspId);
		pRspLogin->rVal  = Msg.Body.RspLogin.rVal;
		pRspLogin->Token = Msg.Body.RspLogin.Token;

		break;
	case ADP_CMD_REQ_LOGOUT:

		break;
	case ADP_CMD_RSP_HEART_BEAT:
		MsgRspHeart_t* pRspHeart;
		rLen      = sizeof(MsgRspHeart_t);
		pRspHeart = (MsgRspHeart_t* )FillMsgHead(MSGT_RSP_HEART_BEAT, pBuf, rLen, Msg.RspId);
		pRspHeart->Token = Msg.Body.RspHeart.Token;
		break;

	case ADP_CMD_REQ_STREAM_OPEN:
		MsgReqStreamOpen_t* pReqStreamOpen;
		rLen           = sizeof(MsgReqStreamOpen_t);
		pReqStreamOpen = (MsgReqStreamOpen_t* )FillMsgHead(MSGT_REQ_STREAM_OPEN, pBuf, rLen, Msg.RspId);
		memcpy(pReqStreamOpen->URL, Msg.Body.ReqStreamOpen.URL, sizeof(pReqStreamOpen->URL));

		break;
	case ADP_CMD_RSP_STREAM_OPEN:
		MsgRspStreamOpen_t* pRspStreamOpen;
		rLen           = sizeof(MsgRspStreamOpen_t);
		pRspStreamOpen = (MsgRspStreamOpen_t* )FillMsgHead(MSGT_RSP_STREAM_OPEN, pBuf, rLen, Msg.RspId);
		pRspStreamOpen->rVal            = Msg.Body.RspStreamOpen.rVal;
		pRspStreamOpen->Token           = Msg.Body.RspStreamOpen.Token;
		pRspStreamOpen->Frames          = Msg.Body.RspStreamOpen.Frames;
		pRspStreamOpen->vWidth          = Msg.Body.RspStreamOpen.vWidth;
		pRspStreamOpen->vHeight         = Msg.Body.RspStreamOpen.vHeight;
		pRspStreamOpen->vBitRate        = Msg.Body.RspStreamOpen.vBitRate;
		pRspStreamOpen->vFrameRate      = Msg.Body.RspStreamOpen.vFrameRate;
		pRspStreamOpen->FrameIntValmSec = Msg.Body.RspStreamOpen.FrameIntValmSec;

		break;
	case ADP_CMD_REQ_STREAM_CLOSE:
		MsgReqStreamClose_t* pReqStreamClose;
		rLen            = sizeof(MsgReqStreamClose_t);
		pReqStreamClose = (MsgReqStreamClose_t* )FillMsgHead(MSGT_REQ_STREAM_CLOSE, pBuf, rLen, Msg.RspId);
		pReqStreamClose->Token = Msg.Body.ReqStreamClose.Token;

		break;
	case ADP_CMD_RSP_STREAM_CLOSE:
		MsgRspStreamClose_t* pRspStreamClose;
		rLen            = sizeof(MsgRspStreamClose_t);
		pRspStreamClose = (MsgRspStreamClose_t* )FillMsgHead(MSGT_RSP_STREAM_CLOSE, pBuf, rLen, Msg.RspId);
		pRspStreamClose->rVal = Msg.Body.RspStreamClose.rVal;

		break;
	case ADP_CMD_REQ_STREAM_SEEK:
		MsgReqStreamSeek_t* pReqStreamSeek;
		rLen           = sizeof(MsgReqStreamSeek_t);
		pReqStreamSeek = (MsgReqStreamSeek_t* )FillMsgHead(MSGT_REQ_STREAM_SEEK, pBuf, rLen, Msg.RspId);
		pReqStreamSeek->Offset = Msg.Body.ReqStreamSeek.Offset;

		break;
	case ADP_CMD_RSP_STREAM_SEEK:
		MsgRspStreamSeek_t* pRspStreamSeek;
		rLen           = sizeof(MsgRspStreamSeek_t);
		pRspStreamSeek = (MsgRspStreamSeek_t* )FillMsgHead(MSGT_RSP_STREAM_SEEK, pBuf, rLen, Msg.RspId);
		pRspStreamSeek->rVal = Msg.Body.RspStreamSeek.rVal;

		break;
	case ADP_CMD_REQ_STREAM_MODE:
		MsgReqStreamMode_t* pReqStreamMode;
		rLen           = sizeof(MsgReqStreamMode_t);
		pReqStreamMode = (MsgReqStreamMode_t* )FillMsgHead(MSGT_REQ_STREAM_MODE, pBuf, rLen, Msg.RspId);
		pReqStreamMode->Mode = Msg.Body.ReqStreamMode.Mode;

		break;
	case ADP_CMD_RSP_STREAM_MODE:
		MsgRspStreamMode_t* pRspStreamMode;
		rLen           = sizeof(MsgRspStreamMode_t);
		pRspStreamMode = (MsgRspStreamMode_t* )FillMsgHead(MSGT_RSP_STREAM_MODE, pBuf, rLen, Msg.RspId);
		pRspStreamMode->rVal = Msg.Body.RspStreamMode.rVal;

		break;
	case ADP_CMD_AV_FRAME:
		Frame_t*      pFrame;
		MsgHead_t*    pHead;
		CStreamBase*  pStream;
		pHead    = (MsgHead_t*)pBuf;
		rLen     = sizeof(Frame_t);
		pBody    = FillMsgHead(MSGT_AV_STREAM_FRAME, pBuf, rLen, Msg.RspId);
		pFrame   = (Frame_t* )pBody;
		pFrame->DataLen = Msg.Id;
		pStream = (CStreamBase* )Msg.Body.pObj;
		S32 rVal;
		rVal = pStream->GetFrame(pFrame);
		if (rVal != MW_SUCC)
		{
			rLen = rVal;
		}
		else
		{
			pHead->Len += pFrame->DataLen;
			rLen += pFrame->DataLen;
		}

		break;
	case ADP_CMD_AV_EOF:
		MsgAvStreamEof_t* pAvStreamEof;
		rLen         = sizeof(MsgAvStreamEof_t);
		pAvStreamEof = (MsgAvStreamEof_t* )FillMsgHead(MSGT_AV_STREAM_EOF, pBuf, rLen, Msg.RspId);
		pAvStreamEof->Token = Msg.Body.AvStreamEof.Token;

		break;
	default:
		break;
	}

	return rLen;
}


U32 CAdapterFY::GetPackLen(S8* pBuf)
{
	MsgHead_t *pHead  = (MsgHead_t *)pBuf;
	return (pHead->Len + sizeof(MsgHead_t));
}


S32 CAdapterFY::DoData(S8* Src, Msg_t &Msg)
{
	MsgHead_t* pHead = (MsgHead_t* )Src;
	//DEBUG_INFO("Recv Net MsgId:%d,\n", pHead->MsgT);
	Msg.RspId = pHead->RspId;
	switch (pHead->MsgT) 
	{
	case MSGT_REQ_LOGIN:
		MsgReqLogin_t* pReqLogin;

		pReqLogin = (MsgReqLogin_t* )(pHead->Data);
		memcpy(Msg.Body.ReqLogin.UsrName, pReqLogin->UsrName, sizeof(Msg.Body.ReqLogin.UsrName));
		memcpy(Msg.Body.ReqLogin.UsrName, pReqLogin->Passwd, sizeof(Msg.Body.ReqLogin.Passwd));
		Msg.Type = ADP_CMD_REQ_LOGIN;

		break;
	case MSGT_RSP_LOGIN:
		MsgRspLogin_t* pRspLogin;

		pRspLogin = (MsgRspLogin_t* )(pHead->Data);
		Msg.Body.RspLogin.rVal  = pRspLogin->rVal;
		Msg.Body.RspLogin.Token = pRspLogin->Token;
		Msg.Type = ADP_CMD_RSP_LOGIN;

		break;
	case MSGT_REQ_LOGOUT:
		MsgReqLogout_t* pReqLogout;

		pReqLogout = (MsgReqLogout_t* )(pHead->Data);
		Msg.Body.ReqLogout.Token = pReqLogout->Token;
		Msg.Type = ADP_CMD_REQ_LOGOUT;

	case MSGT_REQ_STREAM_OPEN:
		MsgReqStreamOpen_t* pReqStreamOpen;

		pReqStreamOpen = (MsgReqStreamOpen_t* )(pHead->Data);
		memcpy(Msg.Body.ReqStreamOpen.URL, pReqStreamOpen->URL, sizeof(Msg.Body.ReqStreamOpen.URL));
		Msg.Type = ADP_CMD_REQ_STREAM_OPEN;

		break;
	case MSGT_RSP_STREAM_OPEN:
		MsgRspStreamOpen_t* pRspStreamOpen;

		pRspStreamOpen = (MsgRspStreamOpen_t* )(pHead->Data);
		Msg.Body.RspStreamOpen.rVal            = pRspStreamOpen->rVal;
		Msg.Body.RspStreamOpen.Token           = pRspStreamOpen->Token;
		Msg.Body.RspStreamOpen.Frames          = pRspStreamOpen->Frames;
		Msg.Body.RspStreamOpen.FrameIntValmSec = pRspStreamOpen->FrameIntValmSec;
		Msg.Body.RspStreamOpen.vWidth          = pRspStreamOpen->vWidth;
		Msg.Body.RspStreamOpen.vHeight         = pRspStreamOpen->vHeight;
		Msg.Body.RspStreamOpen.vBitRate        = pRspStreamOpen->vBitRate;
		Msg.Body.RspStreamOpen.vFrameRate      = pRspStreamOpen->vFrameRate;
		Msg.Type = ADP_CMD_RSP_STREAM_OPEN;

		break;
	case MSGT_REQ_STREAM_CLOSE:
		MsgReqStreamClose_t* pReqStreamClose;

		pReqStreamClose = (MsgReqStreamClose_t* )(pHead->Data);
		Msg.Body.ReqStreamClose.Token = pReqStreamClose->Token;
		Msg.Type = ADP_CMD_REQ_STREAM_CLOSE;

		break;
	case MSGT_RSP_STREAM_CLOSE:
		MsgRspStreamClose_t* pRspStreamClose;

		pRspStreamClose = (MsgRspStreamClose_t* )(pHead->Data);
		Msg.Body.RspStreamClose.rVal = pRspStreamClose->rVal;
		Msg.Type = ADP_CMD_RSP_STREAM_CLOSE;

		break;
	case MSGT_REQ_STREAM_SEEK:
		MsgReqStreamSeek_t* pReqStreamSeek;

		pReqStreamSeek = (MsgReqStreamSeek_t* )(pHead->Data);
		Msg.Body.ReqStreamSeek.Offset = pReqStreamSeek->Offset;
		Msg.Type = ADP_CMD_REQ_STREAM_SEEK;

		break;
	case MSGT_RSP_STREAM_SEEK:
		MsgRspStreamSeek_t* pRspStreamSeek;

		pRspStreamSeek = (MsgRspStreamSeek_t* )(pHead->Data);
		Msg.Body.RspStreamSeek.rVal = pRspStreamSeek->rVal;
		Msg.Type = ADP_CMD_RSP_STREAM_SEEK;

		break;
	case MSGT_REQ_STREAM_MODE:
		MsgReqStreamMode_t* pReqStreamMode;

		pReqStreamMode = (MsgReqStreamMode_t* )(pHead->Data);
		Msg.Body.ReqStreamMode.Mode = pReqStreamMode->Mode;
		Msg.Type = ADP_CMD_REQ_STREAM_MODE;

		break;
	case MSGT_RSP_STREAM_MODE:
		MsgRspStreamMode_t* pRspStreamMode;

		pRspStreamMode = (MsgRspStreamMode_t* )(pHead->Data);
		Msg.Body.RspStreamMode.rVal = pRspStreamMode->rVal;
		Msg.Type = ADP_CMD_RSP_STREAM_MODE;
		
		break;
	case MSGT_RSP_HEART_BEAT:
		MsgRspHeart_t* pRspHeart;
		pRspHeart = (MsgRspHeart_t* )(pHead->Data);
		Msg.Body.RspHeart.Token = pRspHeart->Token;
		Msg.Type = ADP_CMD_RSP_HEART_BEAT;

		break;
	case MSGT_AV_STREAM_FRAME:
		Msg.Type = ADP_CMD_AV_FRAME;
		Msg.Body.pFrame = (Frame_t* )(pHead->Data);

		break;
	case MSGT_AV_STREAM_EOF:
		MsgAvStreamEof_t* pAvStreamEof;
		pAvStreamEof = (MsgAvStreamEof_t* )(pHead->Data);
		Msg.Body.AvStreamEof.Token = pAvStreamEof->Token;
		Msg.Type = ADP_CMD_AV_EOF;

		break;
	default: 
		Msg.Type = ADP_CMD_UNKNOW;
		//SDEBUG_ERROR("msg error : MsgT:%d \n", pHead->MsgT);
		return MW_FAILED;
	}

	return MW_SUCC;
}


Bool CAdapterFY::Parser(Msg_t& Msg)
{
	Bool rVal        = false;
	S8*  pParsePoint = NULL;
	S8*  pCurPos     = NULL;
	U32  DataLen     = 0;
	U32  PacketLen   = 0;
	Msg.Type = ADP::ADP_CMD_UNKNOW;

	do {
		pParsePoint = m_RcvBuf.FindKey("FYMT", sizeof("FYMT"));
		if (pParsePoint) 
		{
			pCurPos = pParsePoint;
		    DataLen =  m_RcvBuf.GetDataLen();
			if (DataLen < (sizeof(MsgHead_t)))
			{
				break;
			}
			PacketLen = GetPackLen(pParsePoint);
			if (DataLen < PacketLen) 
			{
				break;
			}
			if (!m_RcvBuf.CheckDataLen(PacketLen)) 
			{
				rVal = true;
				pCurPos++;
				break;
			}
			
			if (MW_SUCC != DoData(pParsePoint, Msg)) 
			{
				pCurPos = pParsePoint + 1;
			} 
			else 
			{
				pCurPos = pParsePoint + PacketLen;
			}
			rVal = true;
		}
	} while(0);
	m_RcvBuf.SetParsePos(pCurPos);

	return rVal;
}








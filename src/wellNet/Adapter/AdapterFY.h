/***********************************************************************************************************************
Copyright:  
FileName: AdapterFY.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/28/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef ADAPTER_ADAPTERFY_H_
#define ADAPTER_ADAPTERFY_H_
#include "../Common/Common.h"
#include "../Net/RcvBuf.h"
#include "AdapterBase.h"

class WELL_SDK CAdapterFY : public CAdapterBase
{
public:
	CAdapterFY(CRcvBuf& Buf);
	virtual ~CAdapterFY();
	
public:
	virtual S32  Fill(Msg_t& Msg, S8* pBuf, U32 Len);
	virtual Bool Parser(Msg_t& Msg);
	
protected:
	S8*  FillMsgHead(U32 CMD, S8* pBuf, S32& Len, U32 RspId);
	S8*  GetRcvBufWrPos();
	S32  AddRcvBufDataLen(U32 AddLen);
	S32  DoData(S8* Src, Msg_t& Msg);
	U32  GetPackLen(S8* pBuf);
	//Bool ParserSnap(Msg_t* pMsg);

protected:
	CRcvBuf&                        m_RcvBuf;
};


#endif /*ADAPTER_ADAPTERFY_H_*/

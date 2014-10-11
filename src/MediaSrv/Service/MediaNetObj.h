#pragma once
#include "../Net/netobj.h"
class CMediaNetObj :
	public CNetObj
{
public:
	CMediaNetObj(Socket_t sHandle, CCrowd* pCrowd, S32 SndBufSize, S32 RcvBufSize);//Server
	CMediaNetObj(S32 SndBufSize, S32 RcvBufSize); //Client

	virtual ~CMediaNetObj(void);

protected:
	virtual string GetUrl(string Url);
};


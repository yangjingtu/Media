#include "MediaNetObj.h"
#include "CharSetHelper.h"
#include "Config.h"


CMediaNetObj::CMediaNetObj(Socket_t sHandle, CCrowd* pCrowd, S32 SndBufSize, S32 RcvBufSize)
	: CNetObj(sHandle, pCrowd, SndBufSize, RcvBufSize)
{

}

CMediaNetObj::CMediaNetObj(S32 SndBufSize, S32 RcvBufSize)
	: CNetObj(SndBufSize, RcvBufSize)
{

}

CMediaNetObj::~CMediaNetObj(void)
{
}


string CMediaNetObj::GetUrl(string Url)
{
	S8  Ip[20]   = { 0 };
	S32 Port     =   0;
	S8  URL[256] = { 0 };
	sscanf(Url.c_str(), "ip=%[^&]&port=%d&url=%s", Ip, &Port, URL);

	wstring strMediaPath = CONFIG.GetMediaPath();
	string strUrl = W2C(strMediaPath);
	
	string strTU = URL;
	if(strTU.find(strUrl) != string::npos)
	{
		return strTU;
	}
	else
	{
		strUrl += "\\";
		strUrl += URL;
	}
	return strUrl;
}
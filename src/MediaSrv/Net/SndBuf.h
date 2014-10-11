/***********************************************************************************************************************
Copyright:  
FileName: SndBuf.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef NET_SNDBUF_H_
#define NET_SNDBUF_H_
#include "../Common/Common.h"

class CSndBuf
{
public:
	CSndBuf();
	~CSndBuf();

public:
	S32  Init(S32 Size = 1024*1024);
	Void Clear();
	S8*  GetWrPos();
	S8*  GetRdPos();
	S32  DataLen();
	S32  FreeLen();
	S32  IncLen(S32 Len);
	S32  DecLen(S32 Len);

protected:
	S8*  m_pStart;
	S8*  m_pWrPos;
	S8*  m_pRdPos;
	S32  m_BufSize;
	S32  m_DataLen;
};

#endif
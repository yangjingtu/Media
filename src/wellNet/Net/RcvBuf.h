/***********************************************************************************************************************
Copyright:  
FileName: RcvBuf.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/28/8   1.0.0.0     build this file 
************************************************************************************************************************/

#ifndef ADAPTER_RCVBUF_H_
#define ADAPTER_RCVBUF_H_
#include "../Common/Common.h"

class WELL_SDK CRcvBuf
{
public:
	CRcvBuf();
	~CRcvBuf();
	Void Init(S32 hSize = 64, S32 BufSize = 64*1024);
	Void UnInit();
	Void Clear();
public:
	S32  GetFreeLen();
	S32  AddDataLen(S32 AddLen);
	S8*  GetStartPos();
	S8*  GetWrPos();
	S8*  FindKey(const S8* key, S32 len);
	S32  Update();
	S32  GetDataLen();
	Void SetParsePos(S8* Pos);
	Bool CheckDataLen(S32 Len);
protected:
	S8* m_Start;
	S8* m_Buf;
	S8* m_WrPos;
	S8* m_ParsePos;
	S32 m_DataLen;
	S32 m_BufSize;
};

#endif /* ADAPTER_RCVBUF_H_ */

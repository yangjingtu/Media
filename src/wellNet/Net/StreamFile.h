/***********************************************************************************************************************
Copyright:  
FileName: StreamFile.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef NET_STREAMFILE_H_
#define NET_STREAMFILE_H_
#include <string>
#include "StreamBase.h"
#include "../FileMng/AviObj.h"
using namespace std;


class WELL_SDK CStreamFile : public CStreamBase
{
public:
	CStreamFile() { m_pFileObj = new CAviObj; }
	~CStreamFile() { delete m_pFileObj; }

public:
	virtual S32 Open(const char* Name, S32 Mode) { return m_pFileObj->Open(Name, Mode); }
	virtual S32 Close()						{ return m_pFileObj->Close(); }
	virtual S32 SetRdMode(S8 Mode)			{ return m_pFileObj->SetRdMode(Mode); }
	virtual S32 GetFrame(Frame_t *pFrame)   { return m_pFileObj->GetFrame(pFrame); }
	virtual S32 Seek(S32 FrameNo)           { return m_pFileObj->Seek(FrameNo); }
	virtual S32 GetFrames()                 { return m_pFileObj->GetFrames(); }
	virtual S32 GetIntVal()                 { return m_pFileObj->GetIntVal(); }
	virtual S32 GetVHead(VideoHead_t &vHead){ return m_pFileObj->GetVHead(vHead); };

protected:
	CFileObj*   m_pFileObj;
};


#endif /*NET_STREAMFILE_H_*/
/***********************************************************************************************************************
Copyright:  
FileName: AvStreamBase.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef NET_STREAMBASE_H_
#define NET_STREAMBASE_H_
#include <string>
#include "../Common/Common.h"
#include "../FileMng/FileObj.h"
using namespace std;

#define AV_STREAM_MODE            AVI_FILEMODE_READ
#define AV_STREAM_END_DATA        FILE_END_OF_DATA

class CStreamBase
{
public:
	virtual S32 Open(string Name, S32 Mode) = 0;
	virtual S32 Close() = 0;
	virtual S32 SetRdMode(S8 Mode) = 0;
	virtual S32 GetFrame(Frame_t *pFrame) = 0;
	virtual S32 Seek(S32 FrameNo) = 0;
	virtual S32 GetFrames() = 0;
	virtual S32 GetIntVal() = 0;
	virtual S32 GetVHead(VideoHead_t &vHead) = 0;
};

#endif /*NET_STREAMBASE_H_*/
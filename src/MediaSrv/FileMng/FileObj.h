/***********************************************************************************************************************
Copyright:  
FileName: AviObj.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef FILEMNG_FILEOBJ_H_
#define FILEMNG_FILEOBJ_H_
#include <string>
#include "../Common/Common.h"
#include "gmavi_api.h"
using namespace std;

#define AVI_FILEMODE_READ      GMAVI_FILEMODE_READ
#define FILE_END_OF_DATA       -100  

typedef struct FileInfo_
{

}FileInfo_t;

class CFileObj
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

#endif /*FILEMNG_FILEOBJ_H_*/
/***********************************************************************************************************************
Copyright:  
FileName: main.cpp    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/27/8   1.0.0.0     build this file 
************************************************************************************************************************/
#include "Common/CommFunc.h"
#include "Thread/ThreadPoolT.hpp"
#include "Net/NetServer.h"
#include "FileMng/FileObj.h"

class CTestA : public CTrash
{
public:
	CTestA() { }
	virtual ~CTestA() { DEBUG_INFO("func:%s[%d]\n", __FUNCTION__, __LINE__); }
};

class CTestB : public CTrash {
public:
	CTestB() { }
	virtual ~CTestB() { DEBUG_INFO("func:%s[%d]\n", __FUNCTION__, __LINE__); }
};  

/***********************************************************************************************************************
Function:  Init
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 SysInit()
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD( 1, 1 ), &WSAData))
	{
		DEBUG_INFO("WSAStartup init err [%s][%d]\n", __FILE__, __LINE__);
		return MW_FAILED;
	}

	return MW_SUCC;
}

/***********************************************************************************************************************
Function:  UnInit
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 SysUnInit()
{
	WSACleanup();

	return MW_SUCC;
}

#define TEST_FILE_PATH  "E:\\Src\\Video\\" 
Void TestAvi()
{
	/*
	CFileObj   m_FileObj;
	m_FileObj.Open(TEST_FILE_PATH"TestVideo.avi", AVI_FILEMODE_READ);
	FILE* fp = fopen(TEST_FILE_PATH"TestAvi.no", "wb+");

	S8* p = new S8[1024*1024];
	Frame_t* pFrame = (Frame_t*)p;
	pFrame->DataLen = 1024*1024;
	S32 cnt = 0;
	while (1)
	{
		pFrame->DataLen = 1024*1024;
		if(!m_FileObj.GetFrame(pFrame))
		{
			printf("cnt:%d pFrame->DataLen:%d\n", cnt++, pFrame->DataLen);
			fwrite(pFrame->Data, pFrame->DataLen, 1, fp);
		}
		else
		{
			break;
		}
	}

	fclose(fp);
	delete []p;
	while(1)sleep(3);*/
}

//int _tmain(int argc, _TCHAR* argv[])
int win32_main()
{
	//return TestAvi();

	DEBUG_INFO("Media server start...\n");
	SysInit();	

	CNetServer Srv;
	Srv.SetTask(10, 1024 *1024);
	Srv.Start("55555:9527:9088:12345:65432");

	while(1){ sleep(1); };

	SysUnInit();

	return MW_SUCC;
}


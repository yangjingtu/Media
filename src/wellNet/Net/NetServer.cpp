/***********************************************************************************************************************
Copyright:  
FileName: NetServer.cpp    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#include "NetServer.h"
#include "NetObj.h"

/***********************************************************************************************************************
Function: NetServer
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
CNetServer::CNetServer() 
{
	m_pThreadPool          = NULL;
	m_QTaskTHandle         = NULL;
	m_ThreadCtrl.MaxTask   = MAX_THREAD_NUM;
	m_ThreadCtrl.StackSize = 1024;

	while (!QTaskEmpty()) { QTaskPop(); }
}

/***********************************************************************************************************************
Function: 
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
CNetServer::~CNetServer() 
{
}

/***********************************************************************************************************************
Function: 
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::SetTask( U32 MaxTask, U32 StackSize)
{
	m_ThreadCtrl.MaxTask   = MaxTask;
	m_ThreadCtrl.StackSize = StackSize;

	return MW_SUCC;
}

/***********************************************************************************************************************
Function: 
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::SetTimeOut( S32 mSec)
{
	//m_CntT = m_Sec / 10;
	
	return MW_SUCC;
}

/***********************************************************************************************************************
Function: Start
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::Start( std::string Link )
{
	m_Link = Link;
	m_pSock = new CSock;
	ASSERT(m_pSock != NULL );
	if (MW_SUCC != m_pSock->Sock(true, m_Link, true)) 
	{
		DEBUG_INFO("Sock Err %s %d\n", __FUNCTION__, __LINE__);
		return MW_FAILED;
	} 
	m_pThreadPool = new CThreadPoolT<ProcFunc0, Socket_t>(m_ThreadCtrl);
	ASSERT( m_pThreadPool != NULL );
	m_tHandleSrv = ::CreateNormalThread(SrvThreadProc, this, 512 * 1024);

	return MW_SUCC;	
}

/***********************************************************************************************************************
Function: Stop
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::Stop()
{
	::CancelNormalThread(m_tHandleSrv);
	
	if( m_pThreadPool) 
	{
		delete m_pThreadPool;
	}

	if (m_pSock) 
	{
		delete m_pSock;
	}

	return MW_SUCC;
}

/***********************************************************************************************************************
Function: TaskCb
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::TaskCb(Socket_t sHandle, Void *Obj)
{
	if (Obj) 
	{
		CNetServer *pSelf = (CNetServer *)Obj;
		pSelf->DoReq(sHandle);
	}

	return MW_SUCC;
}

/***********************************************************************************************************************
Function: DoReq
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32  CNetServer::DoReq(Socket_t sHandle)
{
	CNetObj Obj(sHandle, &m_Crowd, 1 * 1024 * 1024, 64*1024);

	return Obj.Run();
}

/***********************************************************************************************************************
Function: DoSrvBusy
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
Void  CNetServer::DoSrvBusy(Socket_t sHandle)
{
	DEBUG_INFO("%s %d \n", __FUNCTION__, __LINE__);
	//CSock Sock( fd, false );
	//SendMsg(Sock, L_RCF::MGS_FUNC_SERV_BUSY);
}

/***********************************************************************************************************************
Function: SrvThreadProc
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
tProcType_t CNetServer::SrvThreadProc(tProcPara_t Param)
{
	if (Param) 
	{
		CNetServer *pSlef = (CNetServer *)Param;
		pSlef->SrvThreadBody();
	}

	return NULL;
}

/***********************************************************************************************************************
Function: SrvThreadBody
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::SrvThreadBody()
{
	ASSERT(NULL != m_pSock);
	ASSERT(NULL != m_pThreadPool);
	Socket_t sHandle;
	while (true) 
	{
		sHandle = m_pSock->Accept(); 
		if (SOCK_HANDLE_OK(sHandle)) 
		{
			TaskAdd(sHandle);
		}
	}

	return MW_SUCC;
}

/***********************************************************************************************************************
Function: TaskAdd
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::TaskAdd(Socket_t sHandle)
{
	if (!SOCK_HANDLE_OK(sHandle))
	{
		DEBUG_INFO("sHandle err\n");
		return MW_FAILED;
	}
	
	if (!THREAD_HANDLE_OK(m_QTaskTHandle)) 
	{
		m_QTaskTHandle = ::CreateNormalThread(QTaskProc, this, 512 * 1024);
	}

	if (THREAD_HANDLE_OK(m_QTaskTHandle)) 
	{
		QTaskPush(sHandle);
	}
	else 
	{
		SOCK_CLOSE(sHandle);
	}

	return MW_SUCC;
}

/***********************************************************************************************************************
Function: QTaskEmpty
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
Bool CNetServer::QTaskEmpty( )
{
	return m_QTask.empty();
}

/***********************************************************************************************************************
Function: QTaskSize
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::QTaskSize( )
{
	return m_QTask.size();
}

/***********************************************************************************************************************
Function: QTaskPush
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::QTaskPush(Socket_t sHandle)
{
	do 
	{
		CAutoDoT<CLock> Auto(m_QTaskLock);
		m_QTask.push(sHandle);
	} while (0);

	m_QTaskCond.Wake();
	
	return MW_SUCC;
}

/***********************************************************************************************************************
Function: QTaskPop
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
Socket_t CNetServer::QTaskPop()
{
	Socket_t sHandle = m_QTask.front();
	m_QTask.pop();
	return sHandle;
}

/***********************************************************************************************************************
Function: QTaskProc
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
tProcType_t CNetServer::QTaskProc(tProcPara_t Param)
{
	if (Param) 
	{
		CNetServer *pSlef = (CNetServer *)Param;
		pSlef->QTaskBody();
	}

	return NULL;
}

/***********************************************************************************************************************
Function: QTaskBody
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CNetServer::QTaskBody()
{
	//S32 flag; 
	//S32 ReuseAddr = 1;
	while( true ) {
		do{
			Socket_t sHandle;
			do 
			{
				CAutoDoT<CLock> Auto(m_QTaskLock);
				while (QTaskEmpty()) 
				{ 
					m_QTaskCond.Wait(m_QTaskLock);
				}
				sHandle = QTaskPop();
			} while (0);

			if (!SOCK_HANDLE_OK(sHandle)) 
			{
				continue;
			}
			//U32 Mode = SOCK_CTL_NOBLOCK;
			//SOCK_CTL(sHandle, SOCK_CTL_CMD, &Mode);
			if ( MW_SUCC != m_pThreadPool->TaskAdd(TaskCb, sHandle, this) ) 
			{
				DoSrvBusy(sHandle);
			}
		} while( 0 );
	}

	return MW_SUCC;
}




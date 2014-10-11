/***********************************************************************************************************************
Copyright:  
FileName: NetServer.h   
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef NET_NETSERVER_H_
#define NET_NETSERVER_H_
#include <string>
#include <map>
#include "../Common/Common.h"
#include "../Thread/Cond.h"
#include "../Thread/Lock.h"
#include "../Thread/ThreadPoolT.hpp"
#include "Sock.h"
#include "Usr.h"
using namespace std;

typedef S32 (*MsgInvoke)(S32 *, Void *);

class CNetServer 
{
public:
	CNetServer();
	~CNetServer();

public:
	static tProcType_t QTaskProc(tProcPara_t Param);
	static tProcType_t SrvThreadProc(tProcPara_t Param);
	static S32 TaskCb(Socket_t sHandle, Void *Obj);

public:
	S32 QTaskBody();
	S32 SrvThreadBody();
	S32 SetTask(U32 MaxTask, U32 StackSize);
	S32 SetTimeOut(S32 mSec = 1000 * 10);
	S32 Start(std::string Link);
	S32 Stop();
	S32 DoReq(Socket_t sHandle);
	
protected:
	Void DoSrvBusy(Socket_t sHandle);
	//VOID SendMsg(CSock &S, L_RCF::MsgT_e T);
////////////////////////////////////////////////////////////////////////////////
	S32 TaskAdd(Socket_t sHandle);
	S32 QTaskSize();
	S32 QTaskPush(Socket_t sHandle);
	Socket_t QTaskPop();
	Bool QTaskEmpty();
	
protected:
	S32                                            m_CntT;
	tHandle_t                                      m_tHandleSrv;
	//MsgInvoke                                    m_Invoke;
	//Void                                         *m_Param;
	string                                         m_Link;
	CSock                                         *m_pSock;
	CThreadPoolT<ProcFunc0, Socket_t>             *m_pThreadPool;
	CThreadPoolT<ProcFunc0, Socket_t>::PoolCtrl_t  m_ThreadCtrl;
	////////////////////////////////////////////////////////////////////////////
	tHandle_t                                      m_QTaskTHandle;
	CLock                                          m_QTaskLock;
	CCond                                          m_QTaskCond;
	queue<Socket_t>                                m_QTask;
	CCrowd                                         m_Crowd;
};


#endif  /*NET_NETSERVER_H_*/

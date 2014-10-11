/***********************************************************************************************************************
Copyright:  
FileName: ThreadPoolT.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef THREAD_THREADPOOL_H_
#define THREAD_THREADPOOL_H_
#include <queue>
#include <vector>
#include "../Common/Common.h"
#include "TaskerT.hpp"
#include "AutoDoT.hpp"
#include "Lock.h"
#include "Cond.h"

#define MAX_THREAD_NUM               (100)
#define MAX_ADD_TASK_SEC             (30)

template<typename T1, typename T2>
class CThreadPoolT  
{	
public:
	typedef struct PoolCtrl_
	{
		S32 MaxTask;
		S32 StackSize;
		PoolCtrl_(){ }
		PoolCtrl_(S32 Task, S32 Size) : MaxTask(Task), StackSize(Size){ }
	} PoolCtrl_t;

public:
	CThreadPoolT(PoolCtrl_t Param);
	virtual~CThreadPoolT();
	S32 TaskAdd(T1 Fn, T2 Arg0, Void *Arg1);
	S32 TaskBody();
	static tProcType_t TaskProc(tProcPara_t Param);
	
protected:
	Bool HaveIdleTask();
	Bool QEmpty();
	CTaskerT<T1, T2> QPop();
	S32 QSize();
	S32 QPush(CTaskerT<T1, T2> t);
 
protected:
	S32           m_MaxAddTaskSec;
	tHandle_t     m_tHandle[MAX_THREAD_NUM*2];
	PoolCtrl_t    m_Ctrl;
	CLock         m_QLock;
	CCond         m_QCond;
	std::queue<CTaskerT<T1, T2>>  m_QTasker;
};


/***********************************************************************************************************************
Function:  CThreadPoolT
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
CThreadPoolT<T1, T2>::CThreadPoolT(PoolCtrl_t Param)
:m_Ctrl(Param)
{
	DEBUG_INFO("ThreadPool Num:%d, stack:%d \n", m_Ctrl.MaxTask, m_Ctrl.StackSize );
	m_MaxAddTaskSec = MAX_ADD_TASK_SEC;
	while (!QEmpty()) 
	{ 
		QPop(); 
	}
	for (S32 i = 0; i < m_Ctrl.MaxTask; i++) 
	{	
		m_tHandle[i] = ::CreateNormalThread( TaskProc, this, m_Ctrl.StackSize );
	}
}

/***********************************************************************************************************************
Function:  ~CThreadPoolT
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
CThreadPoolT<T1, T2>::~CThreadPoolT() 
{
	for ( S32 i = 0; i < m_Ctrl.MaxTask; i++ ) {
		//pthread_cond_broadcast ( &m_QCond );
		//pthread_cancel( mTid[i] );
	}

	mSleep( 3 );
}

/***********************************************************************************************************************
Function:  TaskProc
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
tProcType_t CThreadPoolT<T1, T2>::TaskProc(tProcPara_t Param)
{
	tProcType_t Ret = 0;
	if (Param) 
	{
		//static int ThreadCnt = 0;
		//S8 Name[64];
		//sprintf( Name, "Task%d", ThreadCnt++ );
		//::SetThreadName( Name );
		CThreadPoolT<T1, T2> *pSelf = (CThreadPoolT<T1, T2>*)Param;
		pSelf->TaskBody();
	} 
	
	return Ret;
}

/***********************************************************************************************************************
Function:  TaskBody
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
S32 CThreadPoolT<T1, T2>::TaskBody()
{
	//DEBUG_INFO("Start task thread tid:%d" , pthread_self() );
	while (true) 
	{
		do {
			CTaskerT<T1, T2> t;
			do 
			{
				CAutoDoT<CLock> Auto(m_QLock);
				while (QEmpty()) 
				{ 
					m_QCond.Wait(m_QLock);
				} 
				t = QPop();
			} while (0);
			if (t.m_pFn) 
			{  
				t.m_pFn(t.m_Arg0, t.m_Arg1);
			} 
		} while( 0 );
	}

	return MW_SUCC;
}

/***********************************************************************************************************************
Function:  QEmpty
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
Bool CThreadPoolT<T1, T2>::QEmpty()
{
	return m_QTasker.empty();
}

/***********************************************************************************************************************
Function:  QSize
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
S32 CThreadPoolT<T1, T2>::QSize()
{
	return m_QTasker.size();
}

/***********************************************************************************************************************
Function:  QPush
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
S32 CThreadPoolT<T1, T2>::QPush(CTaskerT<T1, T2> t)
{
	m_QTasker.push(t);
	return MW_SUCC;
}

/***********************************************************************************************************************
Function:  QPop
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
CTaskerT<T1, T2> CThreadPoolT<T1, T2>::QPop()
{
	CTaskerT<T1, T2> t = m_QTasker.front();
	m_QTasker.pop();
	return t;
}

/***********************************************************************************************************************
Function:  TaskAdd
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
S32 CThreadPoolT<T1, T2>::TaskAdd(T1 Fn, T2 Arg0, Void *Arg1)
{
	S32 Cnt = 0;
	while (!HaveIdleTask()) 
	{
		if (Cnt++ > 400) 
		{
			DEBUG_INFO("TaskAdd timeout ");
			return MW_FAILED;
		}
		mSleep(20);
	}
	
	do 
	{
		CAutoDoT<CLock> Auto(m_QLock);
		QPush(CTaskerT<T1, T2>(Fn, Arg0, Arg1));
	} while(0);

	m_QCond.Wake();

	return MW_SUCC;
}

/***********************************************************************************************************************
Function:  HaveIdleTask
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T1, typename T2>
Bool CThreadPoolT<T1, T2>::HaveIdleTask()
{
	if ( QSize() >= m_Ctrl.MaxTask ) {
		return false;
	}

	return true;
}


#endif /*THREAD_PTHREADPOOL_H_*/

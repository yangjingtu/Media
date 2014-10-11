/***********************************************************************************************************************
Copyright:  
FileName: TaskerT.hpp    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef THREAD_TASKER_H_
#define THREAD_TASKER_H_
#include "../Common/Common.h"

//Function pointer list
typedef S32 (*ProcFunc0)(Socket_t, Void *);

template<typename T1, typename T2>
class CTaskerT 
{
public:
	CTaskerT(T1 pFn, T2 Arg0, Void *Arg1 ) : m_pFn(pFn), m_Arg0(Arg0), m_Arg1(Arg1){ }
	CTaskerT() : m_pFn(NULL), m_Arg0(0), m_Arg1(NULL){ }
	virtual ~CTaskerT(){}
	/*
	CTaskerT &operator=(const CTaskerT &Right)
	{ 
		m_pFn = Right.m_pFn, m_Arg0 = Right.m_Arg0, m_Arg1 = Right.m_Arg1;
		return this; 
	} */
public:	
	T1    m_pFn;
	T2    m_Arg0;
	Void *m_Arg1;   
};

#endif /*THREAD_TASKER_H_*/
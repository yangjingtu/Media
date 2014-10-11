/***********************************************************************************************************************
Copyright:  
FileName: AutoDoT.hpp    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef THREAD_AUTO_DOT_H_
#define THREAD_AUTO_DOT_H_
#include "../Common/Common.h"
#include "Lock.h"

template<typename T>
class CAutoDoT 
{
public:
	CAutoDoT(T &Ref);
	virtual ~CAutoDoT();

protected:	
	T &m_Ref;
};

/***********************************************************************************************************************
Function:  CAutoDoT
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T>
CAutoDoT<T>::CAutoDoT(T &Ref) : m_Ref(Ref)
{
	m_Ref.Inc();
}

/***********************************************************************************************************************
Function:  ~CAutoDoT
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
template<typename T>
CAutoDoT<T>::~CAutoDoT()
{
	m_Ref.Dec();
}

#endif /*THREAD_AUTO_DOT_H_*/
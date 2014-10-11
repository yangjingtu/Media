/***********************************************************************************************************************
Copyright:  
FileName: Trash.cpp    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#include "Trash.h"
#include "DataType.h"

Bool CTrash::m_bInit = false;
list<CTrash*> CTrash::m_ObjQ;

/***********************************************************************************************************************
Function:  CTrash
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
CTrash::CTrash()
{
	m_bDeletEd    = false;
	m_aLive       = true;
	m_NotALiveCnt = 0;
	m_ObjQ.push_back(this);
	if (!m_bInit)
	{
		init();
	}
}

/***********************************************************************************************************************
Function:  ~CTrash
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
CTrash::~CTrash()
{
	m_aLive = false;
}

/***********************************************************************************************************************
Function:  init
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
Void CTrash::init()
{
	m_bInit = true;
	CreateNormalThread(TrashProc, NULL, 512 * 1024);
}

/***********************************************************************************************************************
Function:  TrashProc
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
tProcType_t CTrash::TrashProc(tProcPara_t Param)
{
	while (m_bInit) {
		while(m_ObjQ.empty()) { mSleep(3*1000); }
		for (list<CTrash*>::iterator it = m_ObjQ.begin(); it != m_ObjQ.end(); ) {
			if ((*it)->m_aLive == false && (*it)->m_NotALiveCnt++ > 10 ) {
				if((*it)->m_bDeletEd == true)
				{
					delete (*it); 
					(*it) = NULL;
				}
				it = m_ObjQ.erase(it);
			} else {
				++it; 
			}
		}
		sleep(3);
	}

	return MW_SUCC;
}

/***********************************************************************************************************************
Function:  Destroy
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
Void CTrash::Delete()
{
	m_aLive     = false; 
	m_bDeletEd  = true; 
}

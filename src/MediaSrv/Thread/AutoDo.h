/***********************************************************************************************************************
Copyright:  
FileName: AutoDo.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    13/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef THREAD_AUTO_DO_H_
#define THREAD_AUTO_DO_H_

class CAutoDo
{
public:
	virtual Void Inc() = 0;
	virtual Void Dec() = 0;
};

#endif /*THREAD_AUTO_DO_H_*/

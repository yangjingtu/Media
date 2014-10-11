/***********************************************************************************************************************
Copyright:  
FileName: CommDef.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/25/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef COMMON_COMM_DEF_H_
#define COMMON_COMM_DEF_H_

#define RUN_WIN32                                  WIN32
 
#define USE_DEBUG_LOG                              1

#define ABS(x) ((x)<0 ? -(x) : (x))


#define ASSERT(exp) ((void)((exp)?1:(printf("ASSERT failed: file:[%s] func: [%s], line [%d]", \
	__FILE__, __FUNCTION__, __LINE__), abort(), 0)))

#endif /*COMMON_COMM_DEF_H_*/
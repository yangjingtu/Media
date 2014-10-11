/***********************************************************************************************************************
Copyright:  
FileName: DataType.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    13/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef COMMON_DATATYPE_H_
#define COMMON_DATATYPE_H_

#define MW_SUCC                                    0

#define MW_FAILED                                 -1

#define ERROR_NO_BASE                            -10000

typedef enum ERROR_NO_
{		
	NET_MSG_FORMAT_FAILED = ERROR_NO_BASE - 0,
	NET_MSG_SEND_ERR      = ERROR_NO_BASE - 1,
	NET_MSG_RCEV_ERR      = ERROR_NO_BASE - 2,
	NET_MSG_TIME_OUT      = ERROR_NO_BASE - 3,
	NET_MSG_OPEN_FILE_ERR = ERROR_NO_BASE - 4,
} ERROR_NO_e;



#endif /*COMMON_DATATYPE_H_*/
/***********************************************************************************************************************
Copyright:  
FileName: Sock.h    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#ifndef NET_SOCK_H_
#define NET_SOCK_H_
#include "../Common/Common.h"
#include <string>
using namespace std;

#define SOCK_ERR

#define MAX_LISTEN_OBJ  64

#define MAX_EPOLL_OBJ   128

class WELL_SDK CSock
{
public:
	CSock(Bool bBlock = false);
	CSock(Socket_t sHandle, Bool bBlock = false);

	void InitSock(Bool bBlock);

	~CSock();
public:
	S32 Sock(Bool bSever, string Link, Bool bBlk = false);
	S32 Send(const S8 *buf, U32 len, S32 flags = 0);
	S32 Recv(S8 *buf, U32 len, S32 flags = 0);
	S32 SetClient(string Link);
	S32 SetServer(string Link);
	S32 Reconnect();
	Void ClearSock();
	Socket_t Accept();
	Socket_t Handle();
	Bool IsAlive();

protected:
	S32 GetHostByName(const char *Name, char *Ip);

protected:
	Socket_t m_sHandle;
	string   m_StrLink;
	Bool     m_bClose;
	Bool     m_bBlock;
};

#endif /*NET_SOCK_H_*/

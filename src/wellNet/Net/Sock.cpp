/***********************************************************************************************************************
Copyright:  
FileName: Sock.cpp    
Author:        Version :          Date:   
Description:    
Version:         
Function List:    
History:               
<author>   <time>   <version >   <desc>       
 ------    14/26/8   1.0.0.0     build this file 
************************************************************************************************************************/
#include "stdafx.h"
#include "Sock.h"

/***********************************************************************************************************************
Function:  CSock
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
CSock::CSock(Bool bBlock)
{
	InitSock(bBlock);
}

/***********************************************************************************************************************
Function:  CSock
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
CSock::CSock(Socket_t sHandle, Bool bBlock) 
{
	m_sHandle = sHandle;
	m_bBlock  = bBlock;
	//DEBUG_ERROR("Sock handle :%d  [%s][%d]", m_sHandle, __FILE__, __LINE__);
	if (!bBlock) 
	{
		U32 Mode = SOCK_CTL_NOBLOCK;
		SOCK_CTL(m_sHandle, SOCK_CTL_CMD, &Mode);
	}
	m_bClose = false;
}

/***********************************************************************************************************************
Function:  ~CSock
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
CSock::~CSock()
{
	m_bClose = true;
	if (SOCK_HANDLE_OK(m_sHandle)) 
	{
		SOCK_CLOSE(m_sHandle);
	}

	m_sHandle = -1;
}

void CSock::InitSock(Bool bBlock)
{
	m_bBlock  = bBlock;
	m_sHandle = socket(AF_INET, SOCK_STREAM, 0);
	if (!SOCK_HANDLE_OK(m_sHandle)) 
	{
		DEBUG_ERROR("Sock handle :%d err [%s][%d]", m_sHandle, __FILE__, __LINE__);
	}

	if (!bBlock)
	{
		U32 Mode = SOCK_CTL_NOBLOCK;
		SOCK_CTL(m_sHandle, SOCK_CTL_CMD, &Mode);
	}

	m_bClose = false;
}

/***********************************************************************************************************************
Function:  Sock
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CSock::Sock(Bool bServer, string Link, Bool bBlk)
{
	S32 rVal = MW_FAILED;

	m_sHandle = socket(AF_INET, SOCK_STREAM, 0);
	if (!SOCK_HANDLE_OK(m_sHandle)) 
	{
		DEBUG_ERROR("Sock handle :%d err", m_sHandle);
		return rVal;
	}
	if (!bBlk)
	{
		U32 Mode = SOCK_CTL_NOBLOCK;
		SOCK_CTL(m_sHandle, SOCK_CTL_CMD, &Mode);
	}

	if (bServer)
	{
		rVal = SetServer(Link); 
	}
	else 
	{
		rVal = SetClient(Link);
	}

	if (MW_SUCC != rVal) 
	{
		SOCK_CLOSE(m_sHandle);
	}

	return rVal;
}

/***********************************************************************************************************************
Function:  GetHostByName
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CSock::GetHostByName(const char *Name, char *Ip)
{
	S32 ret = MW_FAILED;
	if ( NULL == Name || NULL == Ip ) 
	{
		return ret;
	}

	struct hostent *ht = gethostbyname( Name ); 
	if (ht) 
	{
		sprintf_s( Ip, 20, "%d.%d.%d.%d", ht->h_addr_list[0][0] & 0x00ff, \
			ht->h_addr_list[0][1] & 0x00ff, \
			ht->h_addr_list[0][2] & 0x00ff, \
			ht->h_addr_list[0][3] & 0x00ff
			);
	}	

	return ret;
}

Void CSock::ClearSock()
{
	m_bClose = true;
	if (SOCK_HANDLE_OK(m_sHandle)) 
	{
		SOCK_CLOSE(m_sHandle);
	}
	m_sHandle = -1;

	InitSock(m_bBlock);
}

/***********************************************************************************************************************
Function:  SetClient
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CSock::SetClient(string Link)
{
	S8  Ip[20]   = { 0 };
	S32 Port     =   0;
	S8  Url[256] = { 0 };
//	sscanf(Link.c_str(), "ip=%[^&]&port=%d&url=%s", Ip, &Port, Url);
	sscanf_s(Link.c_str(), "ip=%[^&]&port=%d&url=%s", Ip, 20, &Port, Url, 256);


	SOCK_ADDR_IN SrvAddr;
	SrvAddr.sin_family = AF_INET;
	SrvAddr.sin_port = htons(Port);

	unsigned long ip_t = inet_addr(Ip);
	if (ip_t == INADDR_NONE) 
	{
		S8 nip[64] = { 0 };
		GetHostByName(Ip, nip);
		if ( strcmp(nip, "") ) 
		{
			ip_t = inet_addr( nip );
		}
	}
	if (INADDR_NONE == (SrvAddr.sin_addr.s_addr = ip_t)) 
	{
		DEBUG_ERROR("Ip err\n");
		return MW_FAILED;
	}
	//SrvAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1"); 
	if (SOCK_HANDLE_OK(m_sHandle))  
	{
		memset(&(SrvAddr.sin_zero), '\0', 8);
		int rVal = connect(m_sHandle, (struct sockaddr *)&SrvAddr, sizeof(struct sockaddr));
		//DEBUG_INFO("Connect Ret:%d \n", rVal);
		if (rVal < 0) 
		{

			S32 Err = -1;
			S32 Len = sizeof(int);
			S32 Times = 50;
			while (!m_bClose) 
			{
				fd_set wFds;
				FD_ZERO(&wFds);
				FD_SET(m_sHandle, &wFds);
				timeval TimeOut = { 0, 100 * 1000 };
				if (Times-- < 0) 
				{
					DEBUG_ERROR("connect err rVal:%d\n", rVal);
					return MW_FAILED;
				}

				if (select(m_sHandle + 1, NULL, &wFds, NULL, &TimeOut ) > 0) 
				{
					getsockopt(m_sHandle, SOL_SOCKET, SO_ERROR, (char *)&Err, &Len);
					if (Err != 0) 
					{
						DEBUG_ERROR("connect timeout rVal:%d\n", Err);
						return MW_FAILED;			
					} 
					else 
					{
						break;
					}
				}
			}
		}
	}

	if (m_bClose)
	{
		DEBUG_ERROR("Connect failed");
		return MW_FAILED;
	}
	m_StrLink = Link;

	return MW_SUCC;
}

/***********************************************************************************************************************
Function:  CLock
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CSock::SetServer(string Link)
{
	S32 Ret = MW_FAILED;
	if (!SOCK_HANDLE_OK(m_sHandle) || Link.empty()) 
	{
		DEBUG_ERROR("SetServer sock:%d err", m_sHandle);
		return MW_FAILED;
	}
	S32 Port = atoi(Link.c_str());
	SOCK_ADDR_IN AddrSrv;  
	AddrSrv.sin_family = AF_INET;  
	AddrSrv.sin_port   = htons((U16)Port);  
	AddrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	Ret = ::bind(m_sHandle,(SOCK_ADDR*)&AddrSrv, sizeof(SOCK_ADDR));
	if (Ret)  
	{  
		DEBUG_ERROR("bind failed [err:%d]", Ret);
		return Ret; 
	}
	Ret = ::listen(m_sHandle, MAX_LISTEN_OBJ);
	if (Ret)
	{
		DEBUG_ERROR("Listen failed[err:%d]\n", Ret);
		return Ret;
	}

	return MW_SUCC;
}

/***********************************************************************************************************************
Function:  Reconnect
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CSock::Reconnect()
{
	return SetClient(m_StrLink);
}

/***********************************************************************************************************************
Function:  Send
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CSock::Send( const S8 *buf, U32 len, S32 flag )
{
	S32 Size        =  SOCK_HANDLE_BUFFULL;
	timeval TimeOut = { 0, 10 * 1000 };
	fd_set  wFds;
	FD_ZERO(&wFds);
	FD_SET(m_sHandle, &wFds);
	S32 s = select(m_sHandle + 1, NULL, &wFds, NULL, &TimeOut);
	if (s > 0) 
	{
		S32 rVal = send(m_sHandle, buf, len, flag);
		switch (rVal) 
		{
		case SOCKET_ERROR:
			if (errno == EINTR || errno == EAGAIN)
			{
				Size = SOCK_HANDLE_BUFFULL; 
			}
			else
			{
				Size = SOCK_HANDLE_ERROR; 
			}
			break;
		default: 
			Size = rVal; 
			break;
		}
	} 
	else
	{
		Size = SOCK_HANDLE_BUFFULL;
	}
	
	return Size;
}

/***********************************************************************************************************************
Function:  Recv
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
S32 CSock::Recv(S8 *buf, U32 len, S32 flags)
{
	S32 Size        = SOCK_HANDLE_NODATA;
	timeval TimeOut = { 0, 10 * 1000 };
	fd_set  rFds;
	FD_ZERO(&rFds);
	FD_SET(m_sHandle, &rFds);
	S32 s = select(m_sHandle + 1, &rFds, NULL, NULL, &TimeOut);
	if (s > 0) {
		S32 rVal = recv(m_sHandle, buf, len, flags);
		switch (rVal) 
		{
		case 0:              { Size = SOCK_HANDLE_CLOSE; break; }
		case SOCKET_ERROR :  { Size = SOCK_HANDLE_ERROR; break; }
		default:             { Size = rVal; break;}
		}
	}
	
	return Size;
}

/***********************************************************************************************************************
Function:  Accept
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
Socket_t CSock::Accept()
{
	SOCK_ADDR_IN AddrClt;
	S32 Len = sizeof(SOCK_ADDR); 
 
	return ::accept(m_sHandle,(SOCK_ADDR*)&AddrClt,&Len);
}

/***********************************************************************************************************************
Function:  GetFd
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
Socket_t CSock::Handle()
{
	if (SOCK_HANDLE_OK(m_sHandle)) 
	{
		return m_sHandle;
	} 
	else 
	{
		DEBUG_ERROR("[%s][%d] sHandle err\n", __FILE__, __LINE__);
		return MW_FAILED;
	}
}

/***********************************************************************************************************************
Function:  IsAlive
Description:   
Called By:    
Input:          
Output:          
Return:       
Others:        
************************************************************************************************************************/
Bool CSock::IsAlive( )
{
	return true;
}





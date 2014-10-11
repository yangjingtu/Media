#ifndef __INIFILE_H__
#define __INIFILE_H__

#include <Windows.h>

class CIniFile    
{    
public:    
	CIniFile();
	CIniFile(LPCTSTR szFileName);
	virtual ~CIniFile();    

public:    
	// Attributes  
	void SetFileName(LPCTSTR szFileName);
public:    
	// Operations  
	BOOL SetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int nKeyValue);    

	BOOL SetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszKeyValue);  

	int GetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName);    
	DWORD GetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, TCHAR* szKeyValue);    

	BOOL DeleteSection(LPCTSTR lpszSectionName); 
	BOOL DeleteKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName);

private:    
	TCHAR m_szFileName[MAX_PATH]; // .//Config.ini, 如果该文件不存在，则exe第一次试图Write时将创建该文件     
	
	UINT m_unMaxSection; // 最多支持的section数(256)     
	
	UINT m_unSectionNameMaxSize; // section名称长度，这里设为32(Null-terminated)     
	void Init();
};    


#endif //#define __INIFILE_H__

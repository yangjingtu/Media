#include "stdafx.h"
#include "Log.h"
#include <time.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>

namespace lqs
{
	CLog& GetLogger()
	{
		static CLog log;
		return log;
	}

	CLog::CLog(void)
		: m_strFilePrefix("WELL")
		, m_strFileType(".txt")
		, m_bWriteFileError(false)
		, m_bFileOpened(false)
		, m_iRecordNum(0)
		, m_bConsole(false)
	{
		m_iMaxRecordSize = MAXRECORDSIZE;
		m_iMaxRecordFileSize = MAXFILESIZE;
		m_iFileIndex = 0;
		SetPath();
		SetConfig();
		GetFileIndex();
		if (CreateNewLogFile())
		{
			m_bFileOpened = true;
			InitializeCriticalSection(&m_csLogFile);
		}
	}

	CLog::~CLog(void)
	{
		if (m_bFileOpened)
		{
			fclose(m_pCurrentFile);
			m_pCurrentFile = NULL;
			DeleteCriticalSection(&m_csLogFile);
		}
		else
		{
#ifdef _DEBUG
			FreeConsole();
			m_bConsole = false;
#endif
		}
	}

	void CLog::SetPath()
	{
		char szBuff[MAX_PATH] = {0};

		LPITEMIDLIST pidl;
		if (SUCCEEDED( SHGetSpecialFolderLocation ( NULL, CSIDL_COMMON_DESKTOPDIRECTORY, &pidl ))) 
		{
			if (pidl && SHGetPathFromIDList(pidl, szBuff)) 
			{ 
				char *p;
				p = strrchr(szBuff,'\\');
				*p = 0;
				strcat(szBuff, "\\WELLLOGS\\");
				m_strModulePath = szBuff;
				mkdir(szBuff);//#include <direct.h>
			} 
		}
	}

	std::string CLog::GetNewLogFileName(time_t time)
	{
		char timeBuf[201];
		char index[MAXNUMLENGTH];
		if(m_iFileIndex < m_iMaxRecordFileSize)
		{
			m_iFileIndex = m_iFileIndex + 1;
		}
		else
		{
			m_iFileIndex = 1;
		}
		itoa(m_iFileIndex, index, 10);
		strftime(timeBuf, 200, "_%Y-%m-%d_%H-%M-%S_", localtime(&time));
		return m_strModulePath + m_strFilePrefix + timeBuf + index + m_strFileType;
	}

	bool CLog::CreateNewLogFile()
	{
		std::string filename = GetNewLogFileName(time(NULL));
		if (m_pCurrentFile != NULL)
		{
			fclose(m_pCurrentFile);
			m_pCurrentFile = NULL;
		}
		m_pCurrentFile = fopen(filename.c_str(), "w");//if not exist, create it
		if (m_pCurrentFile)
		{
			m_strLastFileName = filename;
			m_iFileSize = 0;
			m_iRecordNum = 0;
		}
		else
		{
#ifdef _DEBUG
			DWORD dErr = 0;
			dErr = GetLastError();
			PrintOutput("error:%d", dErr);
#endif
		}
		WriteFileIndex();
		UpdateConfigFileIndex();

		return m_pCurrentFile != NULL;
	}

	void CLog::Log(const char *format, ...)
	{
		if(!m_bFileOpened)
		{
#ifdef _DEBUG
			int t = 0;
			char out[1024]="";
			va_list ap;
			va_start(ap, format);
			t = vsnprintf(out, 1024, format, ap);
			va_end(ap);
			PrintOutput(out);
#endif
			return;
		}

		EnterCriticalSection(&m_csLogFile);
		va_list args;
		va_start(args, format);
		Log_va_list(m_lLogLevel, format, args);
		va_end(args);
		LeaveCriticalSection(&m_csLogFile);
	}

#ifdef _DEBUG
	void CLog::PrintOutput(const char* format, ...)
	{
		//////////////////////////////////////////////////////////////////////////
		if(!m_bConsole)
		{
			AllocConsole();   
			HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);   
			int hCrt  = _open_osfhandle((long)hOutput, _O_TEXT);
			FILE* hf = _fdopen(hCrt, "w");
			*stdout = *hf;
			m_bConsole = true;
		}

		int numWritten = 0;
		bool writtenError = false;

		char timeBuf[201] = {0};
		time_t tt = time(NULL);
		int iSize = (int)strftime(timeBuf, 200, "%Y%m%d_%H:%M:%S_", localtime(&tt));
		printf(timeBuf);

		numWritten += iSize;
		char recordNumBuf[MAXNUMLENGTH + 3] = {0};
		itoa(++m_iRecordNum, recordNumBuf, 10);
		strcat(recordNumBuf, "---");
		numWritten += strlen(recordNumBuf);
		printf(recordNumBuf);

		//////////////////////////////////////////////////////////////////////////
		int t = 0;
		char out[1024]="";
		va_list ap;
		va_start(ap, format);
		t = vsnprintf(out, 1024, format, ap);
		va_end(ap);
		//printf("%s,%s(),%d: %s/n", file,fun,line,out);
		//////////////////////////////////////////////////////////////////////////
		printf("%s\r\n", out);

		//FreeConsole();
	}
#endif

	void CLog::Log_va_list(LOG_LEVEL logLevel, const char *format, va_list vargs)
	{
		if (!m_bFileOpened || m_iRecordNum > (m_iMaxRecordSize - 1))
		{
			CreateNewLogFile();
		}

		if (m_bFileOpened)
		{
			int iWritten = __Log(m_pCurrentFile, logLevel, format, vargs);
			if (iWritten < 0)
			{
				m_bWriteFileError = true;
			}
			else
			{
				m_iFileSize += iWritten;
			}
		}
	}

	int CLog::_Log(FILE *fp, LOG_LEVEL logLevel, const char* format, ...)
	{
		int iCount = 0;
		va_list args;
		va_start(args, format);
		iCount = __Log(fp, logLevel, format, args);
		va_end(args);

		return iCount;
	}

	int CLog::__Log(FILE *fp, LOG_LEVEL logLevel, const char* format, va_list args)
	{
		int numWritten = 0;
		bool writtenError = false;

		char timeBuf[201] = {0};
		time_t tt = time(NULL);
		int iSize = (int)strftime(timeBuf, 200, "%Y%m%d_%H:%M:%S_", localtime(&tt));
		int iWritten = (int)fwrite(timeBuf, 1, iSize, fp);
		if (iSize != iWritten)
		{
			writtenError = true;
		}
		numWritten += iWritten;

		char recordNumBuf[MAXNUMLENGTH + 3] = {0};
		itoa(++m_iRecordNum, recordNumBuf, 10);
		strcat(recordNumBuf, "---");
		iWritten = (int)fwrite(recordNumBuf, 1, strlen(recordNumBuf), fp);
		if(iWritten < 0)
		{
			writtenError = true;
		}
		else
		{
			numWritten += iWritten;
		}

		iWritten = vfprintf(fp, format, args);
		if (iWritten < 0)
		{
			writtenError = true;
		}
		else
		{
			numWritten += iWritten;
		}

		if (fputs("\n", fp) == EOF)
		{
			writtenError = true;
		}
		numWritten += 2;

		fflush(fp);

		if (writtenError)
		{
			numWritten = -1;
		}

		return numWritten;
	}

	void CLog::SetConfig()
	{
		std::string file = m_strModulePath + "Config.ini";
		char buf[MAXCONTENTLENGTH] = {0};
		char buf2[MAXNUMLENGTH] = {0};
		if(_access(file.c_str(), 0) != -1)   //如果文件存在
		{
			FILE *pFile = fopen(file.c_str(), "r+");
			if(pFile == NULL)
			{
				CreateDefaultConfigFile();
			}
			
			if(pFile != NULL)
			{
				//Max Record Size
				fgets(buf, MAXCONTENTLENGTH, pFile);
				if(buf[0] == 'M' && buf[1] == 'a' && buf[2] == 'x' && buf[3] == 'R' && buf[4] == 'e' && buf[5] == 'c' && buf[6] == 'o' && buf[7] == 'r' && buf[8] == 'd' && buf[9] == 'N' && buf[10] == 'u' && buf[11] == 'm' && buf[12] == ':')
				{
					for(int i = 13; i < strlen(buf); i++)
					{
						if(i < (13 + sizeof(buf2) - 1))
						{
							buf2[i - 13] = buf[i];
						}
					}
					int recordSize = atoi(buf2);
					if(recordSize == 0 || recordSize > MAXRECORDSIZE)
					{
						m_iMaxRecordSize = MAXRECORDSIZE;
					}
					else
					{
						m_iMaxRecordSize = recordSize;
					}
				}
				else
				{
					fclose(pFile);
					pFile = NULL;
					CreateDefaultConfigFile();
					return;
				}

				//Max Record File Size
				memset(buf, 0, sizeof(buf));
				memset(buf2, 0, sizeof(buf2));
				fgets(buf, MAXCONTENTLENGTH, pFile);
				if(buf[0] == 'M' && buf[1] == 'a' && buf[2] == 'x' && buf[3] == 'R' && buf[4] == 'e' && buf[5] == 'c' && buf[6] == 'o' && buf[7] == 'r' && buf[8] == 'd' && buf[9] == 'F' && buf[10] == 'i' && buf[11] == 'l' && buf[12] == 'e' && buf[13] == 'N' && buf[14] == 'u' && buf[15] == 'm' && buf[16] == ':')
				{
					for(int i = 17; i < strlen(buf); i++)
					{
						if(i < (17 + sizeof(buf2) - 1))
						{
							buf2[i - 17] = buf[i];
						}
					}
					int recordFileSize = atoi(buf2);
					if(recordFileSize == 0 || recordFileSize > MAXFILESIZE)
					{
						m_iMaxRecordFileSize = MAXFILESIZE;
					}
					else
					{
						m_iMaxRecordFileSize = recordFileSize;
					}
				}
				else
				{
					fclose(pFile);
					pFile = NULL;
					CreateDefaultConfigFile();
					return;
				}

				fclose(pFile);
				pFile = NULL;
			}
		}
		else
		{
			CreateDefaultConfigFile();
		}
	}

	void CLog::CreateDefaultConfigFile()
	{
		std::string file = m_strModulePath + "Config.ini";
		char buf[MAXCONTENTLENGTH] = {0};
		char bufNum[MAXNUMLENGTH] = {0};
		FILE *pFile = fopen(file.c_str(), "w");
		if (pFile != NULL)
		{
			itoa(m_iMaxRecordSize, bufNum, 10);
			strcpy(buf, "MaxRecordNum:");
			strcat(buf, bufNum);
			fwrite(buf, 1, strlen(buf), pFile);
			fputs("\n", pFile);

			memset(buf, 0, sizeof(buf));
			memset(bufNum, 0, sizeof(bufNum));
			itoa(m_iMaxRecordFileSize, bufNum, 10);
			strcpy(buf, "MaxRecordFileNum:");
			strcat(buf, bufNum);
			fwrite(buf, 1, strlen(buf), pFile);
			fputs("\n", pFile);

			fclose(pFile);
			pFile = NULL;
		}
	}

	void CLog::WriteFileIndex()
	{
		int len = m_strLastFileName.length();
		std::string file = m_strModulePath + "index.ini";
		if(_access(file.c_str(), 0) == -1)
		{
			CreateIndexFile();
		}
		FILE *pFile = fopen(file.c_str(), "r+");
		if (pFile != NULL)
		{
			char buf[MAXCONTENTLENGTH] = {0};
			char bufIndex[MAXNUMLENGTH] = {0};
			char bufFilePath[MAXCONTENTLENGTH] = {0};
			char* p;
			int index = 0;
			int indexTmp = 0;
			int bufLen = 0;
			bool isUpdate = false;
			while(fgets(buf, MAXCONTENTLENGTH, pFile))
			{
				index++;
				bufLen = strlen(buf);
				p = strchr(buf, '-');
				if(p != NULL)
				{
					*p = 0;
					strcpy(bufIndex, buf);
					strcpy(bufFilePath, p + 1);
					indexTmp = atoi(bufIndex);
					if(indexTmp == 0 || indexTmp != index)   //索引号转化不成功，纠错
					{
						memset(buf, 0, sizeof(buf));
						memset(bufIndex, 0, sizeof(bufIndex));
						itoa(index, bufIndex, 10);
						strcpy(buf, bufIndex);
						strcat(buf, "-");
						fprintf(pFile, "%s", buf);
					}
					else if(m_iFileIndex == indexTmp)
					{
						RemoveLogFile(bufFilePath);
						DWORD errNo = GetLastError();
						fseek(pFile, -(bufLen + 1), SEEK_CUR);
						memset(buf, 0, sizeof(buf));
						itoa(indexTmp, bufIndex, 10);
						strcpy(buf, bufIndex);
						strcat(buf, "-");
						strcat(buf, m_strLastFileName.c_str());
						fprintf(pFile, "%s", buf);
						fclose(pFile);
						pFile = NULL;
						isUpdate = true;

						return;
					}
				}
				else   //restore this index     若文件行数比index的最大值还大怎么办？
				{
					memset(buf, 0, sizeof(buf));
					memset(bufIndex, 0, sizeof(bufIndex));
					fseek(pFile, -(bufLen + 1), SEEK_CUR);
					itoa(index, bufIndex, 10);
					strcpy(buf, bufIndex);
					strcat(buf, "-");
					fprintf(pFile, "%s", buf);
				}
			}  //end while

			if(!isUpdate)
			{
				if(index < m_iFileIndex)
				{
					for(++index; index < m_iFileIndex; index++)
					{
						memset(buf, 0, sizeof(buf));
						memset(bufIndex, 0, sizeof(bufIndex));
						itoa(index, bufIndex, 10);
						strcpy(buf, bufIndex);
						strcat(buf, "-");
						fwrite(buf, 1, strlen(buf), pFile);
						fputs("\n", pFile);
					}
					memset(buf, 0, sizeof(buf));
					memset(bufIndex, 0, sizeof(bufIndex));
					itoa(m_iFileIndex, bufIndex, 10);
					strcpy(buf, bufIndex);
					strcat(buf, "-");
					strcat(buf, m_strLastFileName.c_str());
					fwrite(buf, 1, strlen(buf), pFile);
					fputs("\n", pFile);
				}
			}
			
			fclose(pFile);
			pFile = NULL;
		}
	}

	void CLog::UpdateConfigFileIndex()
	{
		std::string file = m_strModulePath + "CurIndexNo.ini";
		char buf[MAXNUMLENGTH] = {0};
		
		FILE *pFile = fopen(file.c_str(), "w");
		if(pFile != NULL)
		{
			itoa(m_iFileIndex, buf, 10);
			fwrite(buf, 1, strlen(buf), pFile);
			fclose(pFile);
			pFile = NULL;
		}
	}

	void CLog::GetFileIndex()
	{
		std::string file = m_strModulePath + "CurIndexNo.ini";
		char buf[MAXNUMLENGTH] = {0};

		if(_access(file.c_str(), 0) != -1)
		{
			FILE *pFile = fopen(file.c_str(), "r");
			if(pFile != NULL)
			{
				fgets(buf, MAXNUMLENGTH, pFile);
				int index = atoi(buf);
				if(index == 0)
				{
					m_iFileIndex = 0;
				}
				else
				{
					m_iFileIndex = index;
				}
				fclose(pFile);
				pFile = NULL;
			}
		}
		else
		{
			FILE *pFile = fopen(file.c_str(), "w");
			if (pFile != NULL)
			{
				memset(buf, 0, sizeof(buf));
				itoa(m_iFileIndex, buf, 10);
				fwrite(buf, 1, strlen(buf), pFile);
				fclose(pFile);
				pFile = NULL;
			}
		}
		
	}

	void CLog::CreateIndexFile()
	{
		int len = m_strLastFileName.length();
		std::string file = m_strModulePath + "index.ini";
		FILE *pFile = fopen(file.c_str(), "w");
		if(pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}

	void CLog::RemoveLogFile(char* path)
	{
		int len = strlen(path);
		char path2[MAX_PATH] = {0};
		strcpy(path2, path);
		if((int)path2[len - 1] == 10)   //如果最后一个字符是换行符，则替换换行符为结束符
		{
			path2[len - 1] = '\0';
		}
		DeleteFile(path2);
	}
}
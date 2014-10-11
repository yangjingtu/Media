#ifndef _LOG_H
#define _LOG_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include <iostream>
#include <string>

#define WRITELOG lqs::GetLogger().Log

namespace lqs
{
	class CLog
	{
	public:
		enum LOG_LEVEL
		{
			NORMAL_LOG,
			DEBUG_LOG,
			TRACE_LOG,
		};

	public:
		CLog(void);
		~CLog(void);
		void Log(const char *format, ...);

	protected:
		void SetLogLevel(LOG_LEVEL logLevel = DEBUG_LOG)
		{
			m_lLogLevel = logLevel;
		}
		void Log_va_list(LOG_LEVEL logLevel, const char* format, va_list vargs);
		void SetLogFileSize(int maxFileSize = 1024 * 1024)
		{
			m_iMaxFileSize = maxFileSize;
		}
		void SetPath();
		int _Log(FILE *fp, LOG_LEVEL logLevel, const char* format, ...);
		int __Log(FILE *fp, LOG_LEVEL logLevel, const char* format, va_list args);
		std::string GetLastFileName();
		std::string GetNewLogFileName(time_t time);
		bool CreateNewLogFile();
		void WirteLastFileName();
		void SetIndex(std::string lastFile);

	private:
		std::string m_strModulePath;
		std::string m_strFilePrefix;
		std::string m_strFileType;
		std::string m_strLastFileName;

		int m_iMaxFileSize;
		int m_iFileSize;
		int m_iIdleFile;

		bool m_bWriteFileError;
		LOG_LEVEL m_lLogLevel;
		FILE *m_pCurrentFile;
		bool m_bFileOpened;
//#ifdef WIN32
		CRITICAL_SECTION m_csLogFile;
//#else
//		pthread_mutex_t m_csLogFile;
//#endif
	};

	extern CLog& GetLogger();
}

#endif
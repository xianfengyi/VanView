/************************************************************************/
/* ��־ģ��������ʦ��ʵ�ֻ����ϸĽ�����                               */
/************************************************************************/

#include "log.h"

#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


/************************************************************************/
/* Define util functions                                                */
/************************************************************************/
static int GetApplicationDirPath(char * applicationDirPath, size_t len)
{
	char szAppPath[MAX_PATH] = "";
	//char szAppDirectory[MAX_PATH] = "";

	if (!applicationDirPath)
		return -1;

	if (!::GetModuleFileNameA(0, szAppPath, sizeof(szAppPath) - 1))
	{
		printf("failed to get module file name!\n");
		return -1;
	}

	// Extract directory
	strncpy_s(applicationDirPath, len, szAppPath, strrchr(szAppPath, '\\') - szAppPath);
	applicationDirPath[strlen(applicationDirPath)] = '\0';

	return 0;
}

char log_component[32] = { 0 };

void SetLogComponent(const char *name)
{
	if (name)
	{
		strncpy_s(log_component, sizeof(log_component), name, _TRUNCATE);
		ILogger *default_logger = GetDefaultLogger();
		if (default_logger) default_logger->SetModuleName(log_component);
	}
}

/************************************************************************/
/* ά�������Ĵ���������.                                                */
/* ��������ʹ�õ����ǲ����ڳ������ʱ����������������Ӵ                 */
/************************************************************************/

template<typename T>
class LogSingletonHelper
{
public:
	LogSingletonHelper()
	{
		//LogHelperֻ��һ��ʵ������ô��û����
		InitializeCriticalSectionAndSpinCount(&lock_, 0x00000400);
	}

	~LogSingletonHelper()
	{
		if (instance_) {
			delete instance_;
			instance_ = nullptr;
		}
		DeleteCriticalSection(&lock_);
	}

	static T *GetInstance()
	{
		if (!instance_) {
			EnterCriticalSection(&lock_);
			if (!instance_) {
				instance_ = new T;
			}
			LeaveCriticalSection(&lock_);
		}
		return instance_;
	}

private:
	static T *instance_;
	static CRITICAL_SECTION lock_;
};

#define DECLARE_LOG_SINGLETON_HELPER(_type_, _target_var_) \
  _type_ *LogSingletonHelper<_type_>::instance_ = nullptr; \
  CRITICAL_SECTION LogSingletonHelper<_type_>::lock_; \
  LogSingletonHelper<_type_> _target_var_;


/************************************************************************/
/* Define log classes                                                   */
/************************************************************************/

class Log
{
public:
	Log(void);
	~Log(void);

	void write(const char *param);
	void write(const wchar_t *param);
	void write(const int param);

private:
	FILE *log_file; //ʹ��C��IOȷ���̰߳�ȫ
};

Log::Log(void)
{
	const unsigned short max_len = 256;
	char tmp[max_len] = { 0 };
	char dir[max_len] = { 0 };

	GetApplicationDirPath(dir, max_len - 1);
	strcat_s(dir, "\\Log");
	CreateDirectoryA((LPCSTR)dir, NULL);  //>create log sub dir

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf(tmp, "\\%04d%02d%02d", sys.wYear, sys.wMonth, sys.wDay);
	strcat_s(dir, tmp);
	CreateDirectoryA((LPCSTR)dir, NULL);  //>create log sub dir

	char log_file_name[max_len] = { 0 };
	sprintf_s(log_file_name, sizeof(log_file_name), "%s\\%02d%02d%02d-pid%d-%s.log",
		dir, sys.wHour, sys.wMinute, sys.wSecond, GetCurrentProcessId(), log_component);
	log_file = fopen(log_file_name, "a");
	if (log_file) {
		fprintf(stderr, "[%s] log file opened: %s\n", log_component, log_file_name);
	}
	else
		fprintf(stderr, "[%s] log file opened error: %d, file: %s\n", log_component, errno, log_file_name);
}

Log::~Log(void)
{
	if (log_file) fclose(log_file);
	fprintf(stderr, "[%s] log file closed", log_component);
}


void Log::write(const char *param)
{
	if (log_file)
	{
		fprintf(log_file, "%s\n", param);
		fflush(log_file);
	}
}

void Log::write(const wchar_t *param)
{
	if (log_file)
	{
		fwprintf(log_file, L"%s\n", param);
		fflush(log_file);
	}
}

void Log::write(const int param)
{
	if (log_file)
	{
		fprintf(log_file, "%d\n", param);
		fflush(log_file);
	}
}

DECLARE_LOG_SINGLETON_HELPER(Log, log_helper);


/************************************************************************/
/* �ⲿ���õ���־����                                                   */
/************************************************************************/

class Logger : public ILogger
{
public:
	Logger();
	~Logger(void);

	virtual void SetModuleName(const char *mod);
	virtual void LogMsg(LogLevel level, const char *function, int line, const char *fmt, ...);
	virtual void LogMsg(LogLevel level, const char *function, int line, const wchar_t *fmt, ...);
	virtual void LogPrint(const char *fmt, ...);
	virtual void LogPrint(const wchar_t *fmt, ...);

	void write(const char *message);
	void write(const wchar_t *message);

private:
	Log *log_;
	char module_name_[16];
};


Logger::Logger()
{
	log_ = log_helper.GetInstance();
	memset(module_name_, 0, sizeof(module_name_));
}


Logger::~Logger(void)
{
}

void Logger::write(const char *message)
{
	if (log_) log_->write(message);
}

void Logger::write(const wchar_t *message)
{
	if (log_) log_->write(message);
}

void Logger::SetModuleName(const char *mod)
{
	if (mod) strncpy_s(module_name_, sizeof(module_name_), mod, _TRUNCATE);
}

/**
* @breif log����
* @param msdx_log_level ��¼����
* @param line ��¼�������ֵ�����
* @param function ��������
* @param fmt ��¼����Ϣ��ʽ�������ǿɱ����
*/
void Logger::LogMsg(LogLevel log_level, const char* function, int line, const char* fmt, ...)
{
	char message[1024] = { 0 };
	{
		va_list ap;
		va_start(ap, fmt);
		vsnprintf_s(message, sizeof(message), fmt, ap);
		va_end(ap);
	}

	char log_time[32] = { 0 };
	char print_time[32] = { 0 };
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		sprintf(log_time, "%02d-%02d %02d:%02d:%02d.%03d",
			sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);
		sprintf(print_time, "%02d:%02d:%02d",
			sys.wHour, sys.wMinute, sys.wSecond);
	}

	char * level = "";
	switch (log_level) {
	case LOG_LEVEL_DEBUG:
		level = "debug";
		break;
	case LOG_LEVEL_INFO:
		level = "info";
		break;
	case LOG_LEVEL_WARN:
		level = "warn";
		break;
	case LOG_LEVEL_ERROR:
		level = "error";
		break;
	}

	//print to screen
	if (log_level >= LOG_LEVEL_WARN) {
		printf_s("\n[%s] [%s] %s: %s\n\n",
			module_name_, print_time, level, message);
	}
	else {
		printf_s("[%s] [%s] %s: %s\n",
			module_name_, print_time, level, message);
	}

	DWORD pid = ::GetCurrentProcessId();
	DWORD tid = ::GetCurrentThreadId();

	//log to file
	char traceBuf[1024];
	_snprintf(traceBuf, sizeof(traceBuf), "%-6s[PID:%5d][TID:%5d][%s][%s] %s(%d): %s",
		level, pid, tid, log_time, module_name_, function, line, message);

	write(traceBuf);
}

void Logger::LogMsg(LogLevel log_level, const char *function, int line, const wchar_t *fmt, ...)
{
	wchar_t message[1024] = { 0 };
	{
		va_list ap;
		va_start(ap, fmt);
		_vsnwprintf_s(message, sizeof(message), fmt, ap);
		va_end(ap);
	}

	wchar_t wfunction[128];
	{
		int len = MultiByteToWideChar(CP_ACP, 0, function, strlen(function), NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, function, strlen(function), wfunction, len);
		wfunction[len] = '\0';
	}
	wchar_t wlogModule[16];
	{
		int len = MultiByteToWideChar(CP_ACP, 0, module_name_, strlen(module_name_), NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, module_name_, strlen(module_name_), wlogModule, len);
		wlogModule[len] = '\0';
	}

	wchar_t log_time[32] = { 0 }; //��¼��־�õĴ����ڵ�ʱ��
	wchar_t print_time[32] = { 0 }; //����ʱ��
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		swprintf(log_time, sizeof(log_time), L"%02d-%02d %02d:%02d:%02d.%03d",
			sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);
		swprintf(print_time, sizeof(print_time), L"%02d:%02d:%02d",
			sys.wHour, sys.wMinute, sys.wSecond);
	}

	wchar_t * level = L"";
	switch (log_level) {
	case LOG_LEVEL_DEBUG:
		level = L"debug";
		break;
	case LOG_LEVEL_INFO:
		level = L"info";
		break;
	case LOG_LEVEL_WARN:
		level = L"warn";
		break;
	case LOG_LEVEL_ERROR:
		level = L"error";
		break;
	}

	DWORD pid = ::GetCurrentProcessId();
	DWORD tid = ::GetCurrentThreadId();

	//print to screen
	if (log_level >= LOG_LEVEL_WARN) {
		wprintf_s(L"\n[%s] [%s] %s: %s\n\n",
			wlogModule, print_time, level, message);
	}
	else {
		wprintf_s(L"[%s] [%s] %s: %s\n",
			wlogModule, print_time, level, message);
	}

	//log to file
	wchar_t traceBuf[1024];
	_snwprintf(traceBuf, sizeof(traceBuf), L"%-6s[PID:%5d][TID:%5d][%s][%s] %s(%d): %s",
		level, pid, tid, log_time, wlogModule, wfunction, line, message);

	write(traceBuf);
}

void Logger::LogPrint(const char* fmt, ...)
{
	char message[1024] = { 0 };
	{
		va_list ap;
		va_start(ap, fmt);
		vsnprintf_s(message, sizeof(message), fmt, ap);
		va_end(ap);
	}

	SYSTEMTIME sys;
	GetLocalTime(&sys);

	printf_s("[%s] [%02d:%02d:%02d] %s\n",
		module_name_, sys.wHour, sys.wMinute, sys.wSecond, message);
}

void Logger::LogPrint(const wchar_t* fmt, ...)
{
	wchar_t message[1024] = { 0 };
	{
		va_list ap;
		va_start(ap, fmt);
		_vsnwprintf_s(message, sizeof(message), fmt, ap);
		va_end(ap);
	}

	wchar_t wlogModule[16];
	{
		int len = MultiByteToWideChar(CP_ACP, 0, module_name_, strlen(module_name_), NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, module_name_, strlen(module_name_), wlogModule, len);
		wlogModule[len] = '\0';
	}

	SYSTEMTIME sys;
	GetLocalTime(&sys);

	wprintf_s(L"[%s] [%02d:%02d:%02d] %s\n",
		wlogModule, sys.wHour, sys.wMinute, sys.wSecond, message);
}

/************************************************************************/
/* Other                                                                */
/************************************************************************/

ILogger * CreateLogger()
{
	return new Logger;
}

DECLARE_LOG_SINGLETON_HELPER(Logger, default_logger_helper);

ILogger * GetDefaultLogger()
{
	return default_logger_helper.GetInstance();
}

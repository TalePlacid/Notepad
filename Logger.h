#ifndef _LOGGER_H //guard
#define _LOGGER_H
#include <afxwin.h>

class Logger {
public:
	Logger();
	~Logger();

	static void Log(CString text);
	static void Clear();
};

#endif // !_LOGGER_H

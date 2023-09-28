#ifndef LOG_H
#define LOG_H

#include <time.h>
#include <stdio.h>

#include "Colors.h"

void LogOpen(const char* argv0);

void LogClose();

void LogBegin(const char* fileName, const char* funcName, const int line);
#undef  LOG_BEGIN
#define LOG_BEGIN() LogBegin(__FILE__, __func__, __LINE__)


void Log(const char* format, ...);

void LogEnd(const char* fileName, const char* funcName, const int line);
#undef  LOG_END
#define LOG_END() LogEnd(__FILE__, __func__, __LINE__);


#endif

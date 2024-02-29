#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdarg.h>

#define LOG_FILE "sorter_log.txt"

#define LOG(format, ...) logToFile(LOG_FILE, "[%s:%d] " format, __FILE__, __LINE__, ##__VA_ARGS__)

void logToFile(const char *filename, const char *format, ...);

#endif /* __LOGGER_H__ */


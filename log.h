#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define __LOG(ll) \
  va_list args;\
  va_start(args, format);\
  _print_log(ll, format, args);\
  va_end(args);

// Log levels
enum LogLevel {
  LOG_ERROR = 1 << 0,
  LOG_WARNING = 1 << 1,
  LOG_NOTICE = 1 << 2,
  LOG_INFO = 1 << 3,
  LOG_DEBUG = 1 << 4,
  LOG_ALL = ~0,
};

// sequences can be found here: http://man7.org/linux/man-pages/man3/strftime.3.html
#define LOG_TIMESTAMP_FORMAT "%X" 
#define LOG_TIMESTAMP_BUFFER_MAX 50
#define LOG_BUFFER_MAX 16000

// Print all logs higher than log level
static unsigned long _loglevel = LOG_ALL;

void set_loglevel(enum LogLevel ll) { _loglevel= ll; }

// Simple enum to string
char *_log_level_name(enum LogLevel ll) {
  switch(ll) {
    case LOG_ERROR:   return " ERROR ";   break;
    case LOG_WARNING: return "WARNING";   break;
    case LOG_NOTICE:  return "NOTICE ";   break;
    case LOG_INFO:    return " INFO  ";   break;
    case LOG_DEBUG:   return " DEBUG ";   break;
    default:          return "UNKOWNN";   break;
  }
}

/*
 * Number of things: 
 * 1: Check if it should print the following log or not according to the global log level
 * 2: Generate timestamp with LOG_TIMESTAMP_FORMAT
 * 3: Compile raw message from format and args
 * $: Print log
 */
void _print_log(enum LogLevel ll, const char *format, va_list args) {
  // No log that is not on LogLevel
  if(!(ll & _loglevel)) return; 

  // Create time stamp
  char timestamp[LOG_TIMESTAMP_BUFFER_MAX];
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(timestamp, LOG_TIMESTAMP_BUFFER_MAX, LOG_TIMESTAMP_FORMAT, timeinfo);

  // Create Message
  char message[LOG_BUFFER_MAX];
  vsprintf(message, format, args);

  // Compile and print
  fprintf(stderr, "[%s][%s] %s\n", timestamp, _log_level_name(ll), message);
}

// Functions for faster access to logger
void logger(enum LogLevel ll, const char *format, ...) { __LOG(ll); }
void log_error(const char *format, ...) { __LOG(LOG_ERROR); }
void log_warn(const char *format, ...) { __LOG(LOG_WARNING); }
void log_notice(const char *format, ...) { __LOG(LOG_NOTICE); }
void log_info(const char *format, ...) { __LOG(LOG_INFO); }
void log_debug(const char *format, ...) { __LOG(LOG_DEBUG); }

// Simple die function
void die(int code, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  exit(code);
}

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <string.h>

enum LogLevel {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
};

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif


// shorten full file path -> filename
#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : \
(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__))
#endif


// ---------- value printing ----------

inline void logPrintValue(const bool &v)
{
    Serial.print(v ? "true" : "false");
}

template<typename T>
inline void logPrintValue(const T &v)
{
    Serial.print(v);
}

template<typename T, typename... Args>
void logPrintValue(const T &first, const Args&... rest)
{
    logPrintValue(first);
    Serial.print(" ");
    logPrintValue(rest...);
}


// ---------- log core ----------

template<typename... Args>
void logMessage(
        const char* level,
        const char* file,
        int line,
        const Args&... args)
{

    Serial.print("[");
    Serial.print(level);
    Serial.print("] ");

    Serial.print("[");
    Serial.print(file);
    Serial.print(":");
    Serial.print(line);
    Serial.print("] ");

    logPrintValue(args...);

    Serial.println();
}


// ---------- macros ----------

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(...) logMessage("ERROR", __FILENAME__, __LINE__, __VA_ARGS__)
#else
#define LOG_ERROR(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOG_WARN(...) logMessage("WARN ", __FILENAME__, __LINE__, __VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(...) logMessage("INFO ", __FILENAME__, __LINE__, __VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(...) logMessage("DEBUG", __FILENAME__, __LINE__, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#endif //LOGGER_H
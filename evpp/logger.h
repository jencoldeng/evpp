#pragma once

#include <iostream>
#include <string>
#include <sstream>

enum class LogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};

typedef void (*LogHandler)(LogLevel level, 
    const char* filename, 
    int line,
    const std::string& message);

void set_log_handler(LogHandler h);

class Logger : public std::stringstream
{
public:
    Logger(LogLevel level, const char* filename, int line):
        level_(level),
        filename_(filename),
        line_(line)
    {
        //NULL
    }

    ~Logger();

private:

    const LogLevel level_;
    const char* filename_;
    const int line_;
};

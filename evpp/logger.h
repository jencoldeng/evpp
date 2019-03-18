#pragma once

#include <iostream>
#include <string>
#include <sstream>

enum class EVPPLogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};

typedef void (*LogHandler)(EVPPLogLevel level, 
    const char* filename, 
    int line,
    const std::string& message);

//设置日志的handler
void set_log_handler(LogHandler h);

//设置日志输出到stdout
void set_log_stdout();

class Logger : public std::stringstream
{
public:
    Logger(EVPPLogLevel level, const char* filepath, int line):
        level_(level),
        filepath_(filepath),
        line_(line)
    {
        //NULL
    }

    ~Logger();

private:

    const EVPPLogLevel level_;
    const char* filepath_;
    const int line_;
};

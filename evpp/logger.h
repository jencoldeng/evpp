#pragma once

#include <iostream>
#include <string>
#include <sstream>

enum class EVPPLogLevel
{
    EVPP_TRACE,
    EVPP_DEBUG,
    EVPP_INFO,
    EVPP_WARN,
    EVPP_ERROR,
    EVPP_FATAL,
};

typedef void (*EVPP_LogHandler_Type)(EVPPLogLevel level, 
    const char* filename, 
    int line,
    const std::string& message);

//设置日志的handler
void set_evpp_log_handler(EVPP_LogHandler_Type h);

//设置日志输出到stdout
void set_evpp_log_stdout();

class EVPP_Logger : public std::stringstream
{
public:
    EVPP_Logger(EVPPLogLevel level, const char* filepath, int line):
        level_(level),
        filepath_(filepath),
        line_(line)
    {
        //NULL
    }

    ~EVPP_Logger();

private:

    const EVPPLogLevel level_;
    const char* filepath_;
    const int line_;
};

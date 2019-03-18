#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>
#include <string.h>

#include <ctime>
#include <iostream>

#include "logger.h"

//日志回调函数
static LogHandler handler = nullptr;

//日志是否输出到stdout
static bool log_stdout{false};

//设置日志的handler
void set_log_handler(LogHandler h)
{
    handler = h;
}

//设置日志输出到stdout
void set_log_stdout()
{
    log_stdout = true;
}

const char* level_str(EVPPLogLevel level)
{
    switch(level)
    {
        case EVPPLogLevel::EVPP_TRACE:
            return "TRACE";
        case EVPPLogLevel::EVPP_DEBUG:
            return "DEBUG";
        case EVPPLogLevel::EVPP_INFO:
            return "INFO";
        case EVPPLogLevel::EVPP_WARN:
            return "WARN";
        case EVPPLogLevel::EVPP_ERROR:
            return "ERROR";
        case EVPPLogLevel::EVPP_FATAL:
            return "FATAL";
    }
    return "UNKNOWN_LEVEL";
}

//线程ID
thread_local pid_t g_thread_id = 0;

//获取线程ID
static pid_t GetTID()
{
#ifndef __NR_gettid
        #error "Must define __NR_gettid for non-x86 platforms"
#endif

        if(g_thread_id!=0) return g_thread_id;

        g_thread_id = syscall(__NR_gettid);
        if(g_thread_id == -1) g_thread_id = getpid();
        return g_thread_id;
}

//----------------------------------
EVPP_Logger::~EVPP_Logger()
{
    const char* filename = strrchr(filepath_, '/');
    if(filename==nullptr || *(++filename)=='\0')
        filename = filepath_;

    if(handler != nullptr)
    {
        handler(level_, filename, line_, this->str());
    }
    else if(log_stdout)
    {
        time_t now = time(nullptr);
        struct tm local_time;
        localtime_r(&now, &local_time);

        //YYYYMMDDHHMM
        char tm_str[64] = {0};
        snprintf(tm_str, sizeof(tm_str), "[%04d-%02d-%02d %02d:%02d:%02d]",
                local_time.tm_year + 1900,
                local_time.tm_mon + 1,
                local_time.tm_mday,
                local_time.tm_hour,
                local_time.tm_min,
                local_time.tm_sec);

        std::cout<<tm_str<<" ["<<level_str(level_)<<"] "
            "["<<GetTID()<<"] "
            "["<<filename<<":"<<line_<<"] "<<this->str()<<"\n";
    }

    return;
}

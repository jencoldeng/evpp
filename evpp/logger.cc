#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>

#include <ctime>
#include <iostream>

#include "logger.h"

static LogHandler handler = nullptr;

void set_log_handler(LogHandler h)
{
    handler = h;
}

const char* level_str(LogLevel level)
{
    switch(level)
    {
        case LogLevel::TRACE:
            return "TRACE";
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
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
Logger::~Logger()
{
    if(handler != nullptr)
    {
        handler(level_, filename_, line_, this->str());
    }
    else
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
            "["<<filename_<<":"<<line_<<"] "
            "["<<GetTID()<<"] "<<this->str()<<"\n";
    }
}

#pragma once

#include "evpp/platform_config.h"


#ifdef GOOGLE_STRIP_LOG

#ifdef __cplusplus
    #define GOOGLE_GLOG_DLL_DECL           // ʹ�þ�̬glog��ʱ�����붨�����
    #define GLOG_NO_ABBREVIATED_SEVERITIES // û�����������,��˵��Ϊ��Windows.h��ͻ

    #include <glog/logging.h>
#endif

#if GOOGLE_STRIP_LOG == 0
    #define LOG_TRACE LOG(INFO)
    #define LOG_DEBUG LOG(INFO)
    #define LOG_INFO  LOG(INFO)
    #define DLOG_TRACE LOG(INFO) << __PRETTY_FUNCTION__ << " this=" << this << " "
#else
    #define LOG_TRACE if (false) LOG(INFO)
    #define LOG_DEBUG if (false) LOG(INFO)
    #define LOG_INFO  if (false) LOG(INFO)
    #define DLOG_TRACE if (false) LOG(INFO)
#endif

#if GOOGLE_STRIP_LOG <= 1
    #define LOG_WARN  LOG(WARNING)
    #define DLOG_WARN LOG(WARNING) << __PRETTY_FUNCTION__ << " this=" << this << " "
    #else
    #define LOG_WARN  if (false) LOG(WARNING)
    #define DLOG_WARN if (false) LOG(WARNING)
#endif

#define LOG_ERROR LOG(ERROR)
#define LOG_FATAL LOG(FATAL)

#else

#include "evpp/logger.h"

#define LOG_TRACE EVPP_Logger(EVPPLogLevel::EVPP_TRACE, __FILE__, __LINE__)
#define DLOG_TRACE EVPP_Logger(EVPPLogLevel::EVPP_TRACE, __FILE__, __LINE__)
#define LOG_DEBUG EVPP_Logger(EVPPLogLevel::EVPP_DEBUG, __FILE__, __LINE__)
#define LOG_INFO EVPP_Logger(EVPPLogLevel::EVPP_INFO, __FILE__, __LINE__)
#define LOG_WARN EVPP_Logger(EVPPLogLevel::EVPP_WARN, __FILE__, __LINE__)
#define DLOG_WARN EVPP_Logger(EVPPLogLevel::EVPP_WARN, __FILE__, __LINE__)
#define LOG_ERROR EVPP_Logger(EVPPLogLevel::EVPP_ERROR, __FILE__, __LINE__)
#define LOG_FATAL EVPP_Logger(EVPPLogLevel::EVPP_FATAL, __FILE__, __LINE__)
#define CHECK_NOTnullptr EVPP_Logger(EVPPLogLevel::EVPP_ERROR, __FILE__, __LINE__)<< "'" #val "' Must be non nullptr";

#endif

//#ifdef _DEBUG
//#ifdef assert
//#undef assert
//#endif
//#define assert(expr)  { if (!(expr)) { LOG_FATAL << #expr ;} }
//#endif
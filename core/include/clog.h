#pragma once

#include <stdio.h>

#ifdef __ANDROID__
#include <android/log.h>
#define CLogD(...)		__android_log_print(ANDROID_LOG_DEBUG, "NDK-debug", __VA_ARGS__)
#define CLogE(...)		__android_log_print(ANDROID_LOG_ERROR, "NDK-error", __VA_ARGS__)
#elif defined(_WIN32) or defined(linux)
#define CLogD(msg, ...)		printf("CLog [Debug] : " msg "\n", __VA_ARGS__)
#define CLogW(msg, ...)		printf("CLog [Warning] : " msg "\n", __VA_ARGS__)
#define CLogE(msg, ...)		printf("CLog [Error] : " msg "\n", __VA_ARGS__)
#endif
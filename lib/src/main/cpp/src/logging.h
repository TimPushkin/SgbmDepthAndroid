#ifndef SGBM_ANDROID_CORE_LOGGING_H
#define SGBM_ANDROID_CORE_LOGGING_H

#if __has_include(<android/log.h>)

#include <android/log.h>

#define LOGV(tag, ...) __android_log_print(ANDROID_LOG_VERBOSE, tag, __VA_ARGS__)
#define LOGD(tag, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, __VA_ARGS__)
#define LOGI(tag, ...) __android_log_print(ANDROID_LOG_INFO, tag, __VA_ARGS__)
#define LOGW(tag, ...) __android_log_print(ANDROID_LOG_WARN, tag, __VA_ARGS__)
#define LOGE(tag, ...) __android_log_print(ANDROID_LOG_ERROR, tag, __VA_ARGS__)

#else

// TODO: add logging for non-Android platforms

#define LOGV(tag, ...)
#define LOGD(tag, ...)
#define LOGI(tag, ...)
#define LOGW(tag, ...)
#define LOGE(tag, ...)

#endif

#endif

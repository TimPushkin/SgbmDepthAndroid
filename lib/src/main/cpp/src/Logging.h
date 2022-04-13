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

#include <cstdio>

#define LOGV(tag, format, ...) std::fprintf(stderr, "V " tag ": " format "\n", ##__VA_ARGS__)
#define LOGD(tag, format, ...) std::fprintf(stderr, "D " tag ": " format "\n", ##__VA_ARGS__)
#define LOGI(tag, format, ...) std::fprintf(stderr, "I " tag ": " format "\n", ##__VA_ARGS__)
#define LOGW(tag, format, ...) std::fprintf(stderr, "W " tag ": " format "\n", ##__VA_ARGS__)
#define LOGE(tag, format, ...) std::fprintf(stderr, "E " tag ": " format "\n", ##__VA_ARGS__)

#endif

#endif

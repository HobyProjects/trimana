#pragma once

#include <spdlog/spdlog.h>

#ifdef _DEBUG
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)
#define LOG_INFO(...) spdlog::info(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
#else
#define LOG_CRITICAL(...) 
#define LOG_INFO(...)
#define LOG_ERROR(...)
#define LOG_WARN(...)
#endif
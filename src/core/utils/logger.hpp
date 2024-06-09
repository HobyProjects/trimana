#pragma once

#include <spdlog/spdlog.h>
#include "platform.hpp"

namespace TrimanaCore
{
    class Log
    {
        public:
            static void Init();
            static std::shared_ptr<spdlog::logger>& GetCoreLogger(){ return mCoreLogger; }
            static std::shared_ptr<spdlog::logger>& GetClientLogger(){ return mClientLogger; }

        private:
            static std::shared_ptr<spdlog::logger> mCoreLogger;
            static std::shared_ptr<spdlog::logger> mClientLogger;
        
    };
}

#define TRIMANA_CORE_TRACE(...)     TrimanaCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TRIMANA_CORE_INFO(...)      TrimanaCore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TRIMANA_CORE_WARN(...)      TrimanaCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TRIMANA_CORE_ERROR(...)     TrimanaCore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TRIMANA_CORE_CRITICAL(...)  TrimanaCore::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define TRIMANA_TRACE(...)          TrimanaCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TRIMANA_INFO(...)           TrimanaCore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TRIMANA_WARN(...)           TrimanaCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TRIMANA_ERROR(...)          TrimanaCore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TRIMANA_CRITICAL(...)       TrimanaCore::Log::GetCoreLogger()->critical(__VA_ARGS__)
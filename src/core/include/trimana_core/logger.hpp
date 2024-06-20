#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

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
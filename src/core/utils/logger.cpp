#include "logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> TrimanaCore::Log::mCoreLogger;
std::shared_ptr<spdlog::logger> TrimanaCore::Log::mClientLogger;

void TrimanaCore::Log::Init()
{
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Trimana.log", true));

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    mCoreLogger = std::make_shared<spdlog::logger>("TrimanaCore", begin(logSinks), end(logSinks));
    spdlog::register_logger(mCoreLogger);
    mCoreLogger->set_level(spdlog::level::trace);
    mCoreLogger->flush_on(spdlog::level::trace);

    mClientLogger = std::make_shared<spdlog::logger>("TrimanaEngine", begin(logSinks), end(logSinks));
    spdlog::register_logger(mClientLogger);
    mClientLogger->set_level(spdlog::level::trace);
    mClientLogger->flush_on(spdlog::level::trace);
}
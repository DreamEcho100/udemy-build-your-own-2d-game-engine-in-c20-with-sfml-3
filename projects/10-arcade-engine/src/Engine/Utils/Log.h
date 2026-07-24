#pragma once

#include <spdlog/spdlog.h>

#define LOG_INFO(...) SPDLOG_INFO("Engine: " __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN("Engine" __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR("Engine" __VA_ARGS__)

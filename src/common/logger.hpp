/**
 * @file logger.hpp
 * @brief Logging utility for Streaming Device firmware
 */

#pragma once

#include <sstream>
#include <string>
#include <cstdio>
#include <ctime>

namespace streaming::common {

enum class LogLevel : uint8_t {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    void setLevel(LogLevel level) { level_ = level; }
    LogLevel getLevel() const { return level_; }

    void log(LogLevel level, const char* tag, const std::string& msg) {
        if (level < level_) return;
        const char* level_str = levelToString(level);
        time_t now = time(nullptr);
        struct tm* t = localtime(&now);
        char time_buf[32];
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", t);
        fprintf(stderr, "[%s] [%s] [%s] %s\n", time_buf, level_str, tag, msg.c_str());
    }

    template<typename... Args>
    void debug(const char* tag, Args&&... args) {
        log(LogLevel::DEBUG, tag, format(args...));
    }

    template<typename... Args>
    void info(const char* tag, Args&&... args) {
        log(LogLevel::INFO, tag, format(args...));
    }

    template<typename... Args>
    void warn(const char* tag, Args&&... args) {
        log(LogLevel::WARN, tag, format(args...));
    }

    template<typename... Args>
    void error(const char* tag, Args&&... args) {
        log(LogLevel::ERROR, tag, format(args...));
    }

private:
    Logger() : level_(LogLevel::INFO) {}
    LogLevel level_;

    static const char* levelToString(LogLevel l) {
        switch (l) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERROR: return "ERROR";
        }
        return "UNKNOWN";
    }

    static std::string format() { return ""; }

    template<typename T>
    static std::string format(const T& t) {
        std::ostringstream oss;
        oss << t;
        return oss.str();
    }

    template<typename T, typename... Args>
    static std::string format(const T& t, Args&&... args) {
        return format(t) + " " + format(std::forward<Args>(args)...);
    }
};

#define LOG_DEBUG(tag, ...) streaming::common::Logger::instance().debug(tag, __VA_ARGS__)
#define LOG_INFO(tag, ...)  streaming::common::Logger::instance().info(tag, __VA_ARGS__)
#define LOG_WARN(tag, ...)  streaming::common::Logger::instance().warn(tag, __VA_ARGS__)
#define LOG_ERROR(tag, ...) streaming::common::Logger::instance().error(tag, __VA_ARGS__)

} // namespace streaming::common

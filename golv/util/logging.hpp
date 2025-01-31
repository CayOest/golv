#pragma once

#include <iostream>
#include <sstream>

namespace golv {
enum class log_level
{
    fatal,
    error,
    warn,
    debug,
    info,
    trace
};

struct logger
{
    log_level level_ = log_level::trace;

    virtual void log(log_level lvl, const std::string& msg) const = 0;
    void set_level(log_level lvl) { level_ = lvl; }
};

struct console_logger : public logger
{
    void log(log_level lvl, const std::string& msg) const override
    {
        if (static_cast<int>(lvl) > static_cast<int>(level_))
            return;

        switch (lvl) {
            case log_level::fatal:
                std::cout << "[FATAL] " << msg;
                break;
            case log_level::error:
                std::cout << "[ERROR] " << msg;
                break;
            case log_level::warn:
                std::cout << "[WARN ] " << msg;
                break;
            case log_level::debug:
                std::cout << "[DEBUG] " << msg;
                break;
            case log_level::info:
                std::cout << "[INFO ] " << msg;
                break;
            case log_level::trace:
                std::cout << "[TRACE] " << msg;
                break;
            default:
                std::cout << "Unknown log_level";
        }
    }
};

struct the_logger
{
    static logger& instance()
    {
        static console_logger _instance;
        return _instance;
    }
};

void
set_log_level(log_level lvl);
}

#define _GOLV_LOG(lvl, msg)                                                                                            \
    do {                                                                                                               \
        std::stringstream ss;                                                                                          \
        ss << msg << std::endl;                                                                                        \
        golv::the_logger::instance().log(lvl, ss.str());                                                               \
    } while (false)

#define GOLV_LOG_FATAL(msg) _GOLV_LOG(golv::log_level::fatal, msg)
#define GOLV_LOG_ERROR(msg) _GOLV_LOG(golv::log_level::error, msg)
#define GOLV_LOG_WARN(msg) _GOLV_LOG(golv::log_level::warn, msg)
#define GOLV_LOG_DEBUG(msg) _GOLV_LOG(golv::log_level::debug, msg)
#define GOLV_LOG_INFO(msg) _GOLV_LOG(golv::log_level::info, msg)
#define GOLV_LOG_TRACE(msg) _GOLV_LOG(golv::log_level::trace, msg)

namespace golv::experimental {

enum class log_level { fatal, error, warn, debug, info, trace };

struct logger {
  log_level level_ = log_level::trace;

  virtual void log(log_level lvl, const std::string& msg) const = 0;
  void set_level(log_level lvl) { level_ = lvl; }
};

struct console_logger : public logger {
  void log(log_level lvl, const std::string& msg) const override {
    if (static_cast<int>(lvl) > static_cast<int>(level_)) return;

    switch (lvl) {
      case log_level::fatal:
        std::cout << "[FATAL] " << msg;
        break;
      case log_level::error:
        std::cout << "[ERROR] " << msg;
        break;
      case log_level::warn:
        std::cout << "[WARN ] " << msg;
        break;
      case log_level::debug:
        std::cout << "[DEBUG] " << msg;
        break;
      case log_level::info:
        std::cout << "[INFO ] " << msg;
        break;
      case log_level::trace:
        std::cout << "[TRACE] " << msg;
        break;
      default:
        std::cout << "Unknown log_level";
    }
  }
};

struct the_logger {
  static logger& instance() {
    static console_logger _instance;
    return _instance;
  }
};

// 🔥 **Allgemeine Log-Funktion**
template <log_level L, typename... Args>
constexpr void log(Args&&... args) {
  std::stringstream ss;
  (ss << ... << args) << std::endl;
  the_logger::instance().log(L, ss.str());
}

// 🎯 **Ersetzte Makros durch Funktionen**
template <typename... Args>
constexpr void log_fatal(Args&&... args) {
  log<log_level::fatal>(std::forward<Args>(args)...);
}
template <typename... Args>
constexpr void log_error(Args&&... args) {
  log<log_level::error>(std::forward<Args>(args)...);
}
template <typename... Args>
constexpr void log_warn(Args&&... args) {
  log<log_level::warn>(std::forward<Args>(args)...);
}
template <typename... Args>
constexpr void log_debug(Args&&... args) {
  log<log_level::debug>(std::forward<Args>(args)...);
}
template <typename... Args>
constexpr void log_info(Args&&... args) {
  log<log_level::info>(std::forward<Args>(args)...);
}
template <typename... Args>
constexpr void log_trace(Args&&... args) {
  log<log_level::trace>(std::forward<Args>(args)...);
}
}  // namespace golv::experimental
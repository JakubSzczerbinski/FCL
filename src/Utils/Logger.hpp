#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <initializer_list>
#include <string>
#include <vector>

#ifndef LOG_LEVEL
  #define LOG_LEVEL 1
#endif

#if LOG_LEVEL <= 0
  #define debug_log(...) log(__FUNCTION__, "DEBUG", __VA_ARGS__)
#else
  #define debug_log(...) log()
#endif
#if LOG_LEVEL <= 1
  #define info_log(...) log(__FUNCTION__, "INFO", __VA_ARGS__)
#else
  #define info_log(...) log()
#endif
#if LOG_LEVEL <= 2
  #define warn_log(...) log(__FUNCTION__, "WARNING", __VA_ARGS__)
#else
  #define warn_log(...) log()
#endif
#if LOG_LEVEL <= 3
  #define error_log(...) log(__FUNCTION__, "ERROR", __VA_ARGS__)
#else
  #define error_log(...) log()
#endif

inline std::string to_string(const std::string& s)
{
  return s;
}

inline std::string to_string(const char* s)
{
  return s;
}

template<typename T>
inline std::string to_string(const T& t)
{
  return std::to_string(t);
}

class Logger {
 public:
  Logger(const std::initializer_list<std::string>& origin) : origin_{origin} {}
  Logger(const std::string& origin) : Logger{origin} {}
  template <typename ... Ms>
  Logger& log(std::string func_name, std::string log_type, const Ms& ... messages)
  {
    fprintf(stderr, "[%s] ", log_type.c_str());
    for (auto& o : origin_)
    {
      fprintf(stderr, "%s/", o.c_str());
    }
    fprintf(stderr, " %s: ", func_name.c_str());
    printLogMessage(messages ...);
    fprintf(stderr, "\n");
    return *this;
  }
  Logger& log()
  {
    return *this;
  }
 private:
  template<typename M, typename ... Ms>
  Logger& printLogMessage(const M& msg, const Ms& ... messages)
  {
    fprintf(stderr, "%s ", to_string(msg).c_str());
    printLogMessage(messages ...);
    return *this;
  }
  template <typename M>
  Logger& printLogMessage(const M& msg)
  {
    fprintf(stderr, "%s ", to_string(msg).c_str());
    return *this;
  }
  std::vector<std::string> origin_;
};

#endif
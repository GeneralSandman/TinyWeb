#ifndef LOG_H
#define LOG_H
#include <fstream>
#include <iostream>
typedef enum log_level {
  Debug = 0,
  Info,
  Warning,
  Error,
  Fatal
} log_level;

void initLogger(const std::string &debug_log_filename,
                const std::string &info_log_filename,
                const std::string &warn_log_filename,
                const std::string &error_log_filename,
                const std::string &fatal_log_filename,
                log_level level);
void setLogLevel(log_level level);

class Logger
{
private:
  static std::ofstream m_nNullStream;

  static std::ofstream m_nDebugLogFile;
  static std::ofstream m_nInfoLogFile;
  static std::ofstream m_nWarnLogFile;
  static std::ofstream m_nErrLogFile;
  static std::ofstream m_nFatalLogFile;

  static log_level m_nLevel;
  Logger()
  {
    // std::cout << "Logger construct\n";
  }

public:
  static Logger &getLoggerInstance()
  {
    // if (m_pLoggerInstance == nullptr)
    //   m_pLoggerInstance = new Logger();
    // return m_pLoggerInstance;

    static Logger LoggerInstance;
    return LoggerInstance;
  }
  std::ostream &getStream(log_level level);
  std::ostream &log(log_level level,
                    const int line,
                    const std::string &function);
  friend void initLogger(const std::string &debug_log_filename,
                         const std::string &info_log_filename,
                         const std::string &warn_log_filename,
                         const std::string &error_log_filename,
                         const std::string &fatal_log_filename,
                         log_level level = Debug);

  friend void setLogLevel(log_level level)
  {
    m_nLevel = level;
  }
  ~Logger();
};

#define LOG(level) \
  Logger::getLoggerInstance().log(level, __LINE__, __FUNCTION__)

#endif
/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.generalsandman.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <iostream>

typedef enum log_level {
    Debug = 0,
    Info,
    Warn,
    Error,
    Fatal
} log_level;

log_level convertStringToLoglevel(const std::string&);

void      initLogger(const std::string& debug_log_filename,
                     const std::string& info_log_filename,
                     const std::string& warn_log_filename,
                     const std::string& error_log_filename,
                     const std::string& fatal_log_filename,
                     log_level          level);
void      setLogLevel(log_level level);

void      setDebugLogFile(const std::string&);
void      setInfoLogFile(const std::string&);
void      setWarnLogFile(const std::string&);
void      setErrorLogFile(const std::string&);
void      setFatalLogFile(const std::string&);

class Logger {
private:
    static std::ofstream m_nNullStream;

    static std::ofstream m_nDebugLogFile;
    static std::ofstream m_nInfoLogFile;
    static std::ofstream m_nWarnLogFile;
    static std::ofstream m_nErrLogFile;
    static std::ofstream m_nFatalLogFile;

    static log_level     m_nLevel;
    Logger() {
        // std::cout << "Logger construct\n";
    }

public:
    static Logger& getLoggerInstance() {
        // if (m_pLoggerInstance == nullptr)
        //   m_pLoggerInstance = new Logger();
        // return m_pLoggerInstance;

        static Logger LoggerInstance;
        return LoggerInstance;
    }
    std::ostream& getStream(log_level level);
    std::ostream& log(log_level          level,
                      const int          line,
                      const std::string& function);
    friend void   initLogger(const std::string& debug_log_filename,
                             const std::string& info_log_filename,
                             const std::string& warn_log_filename,
                             const std::string& error_log_filename,
                             const std::string& fatal_log_filename,
                             log_level          level = Debug);

    friend void   setLogLevel(log_level level) {
          // Logger logger = getLoggerInstance();
        Logger::m_nLevel = level;
    }
    friend void setDebugLogFile(const std::string& file) {
        if (m_nDebugLogFile.is_open())
            m_nDebugLogFile.close();
        m_nDebugLogFile.open(file.c_str(),
                             std::ios::in | std::ios::app);
        if (!m_nDebugLogFile.is_open())
            std::cout << "can't open (" << file << ")\n";
    }
    friend void setInfoLogFile(const std::string& file) {
        if (m_nInfoLogFile.is_open())
            m_nInfoLogFile.close();
        m_nInfoLogFile.open(file.c_str(),
                            std::ios::in | std::ios::app);

        if (!m_nInfoLogFile.is_open())
            std::cout << "can't open (" << file << ")\n";
    }
    friend void setWarnLogFile(const std::string& file) {
        if (m_nWarnLogFile.is_open())
            m_nWarnLogFile.close();
        m_nWarnLogFile.open(file.c_str(),
                            std::ios::in | std::ios::app);

        if (!m_nWarnLogFile.is_open())
            std::cout << "can't open (" << file << ")\n";
    }
    friend void setErrorLogFile(const std::string& file) {
        if (m_nErrLogFile.is_open())
            m_nErrLogFile.close();
        m_nErrLogFile.open(file.c_str(),
                           std::ios::in | std::ios::app);

        if (!m_nErrLogFile.is_open())
            std::cout << "can't open (" << file << ")\n";
    }
    friend void setFatalLogFile(const std::string& file) {
        if (m_nFatalLogFile.is_open())
            m_nFatalLogFile.close();
        m_nFatalLogFile.open(file.c_str(),
                             std::ios::in | std::ios::app);

        if (!m_nFatalLogFile.is_open())
            std::cout << "can't open (" << file << ")\n";
    }
    ~Logger();
};

#define LOG(level) \
    Logger::getLoggerInstance().log(level, __LINE__, __FUNCTION__)

#endif
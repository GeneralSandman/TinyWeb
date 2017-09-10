#include "log.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>


std::ofstream Logger::m_nNullStream(0);

std::ofstream Logger::m_nDebugLogFile;
std::ofstream Logger::m_nInfoLogFile;
std::ofstream Logger::m_nErrLogFile;
std::ofstream Logger::m_nWarnLogFile;
std::ofstream Logger::m_nFatalLogFile;

log_level Logger::m_nLevel = Debug;

void initLogger(const std::string &debug_log_filename,
                const std::string &info_log_filename,
                const std::string &warn_log_filename,
                const std::string &error_log_filename,
                const std::string &fatal_log_filename,
                log_level level)
{
    Logger::m_nDebugLogFile.open(debug_log_filename.c_str(), std::ios::in | std::ios::app);
    Logger::m_nInfoLogFile.open(info_log_filename.c_str(), std::ios::in | std::ios::app);
    Logger::m_nWarnLogFile.open(warn_log_filename.c_str(), std::ios::in | std::ios::app);
    Logger::m_nErrLogFile.open(error_log_filename.c_str(), std::ios::in | std::ios::app);
    Logger::m_nFatalLogFile.open(fatal_log_filename.c_str(), std::ios::in | std::ios::app);

    Logger::m_nLevel = level;
}

std::ostream &Logger::getStream(log_level level)
{

    if (Debug == level)
    {
        if (m_nDebugLogFile.is_open())
        {
            return m_nDebugLogFile;
        }
        else
        {
            return std::cout;
        }
    }
    if (Info == level)
    {
        if (m_nInfoLogFile.is_open())
        {
            return m_nInfoLogFile;
        }
        else
        {
            return std::cout;
        }
    }
    else if (Warning == level)
    {
        if (m_nWarnLogFile.is_open())
        {
            return m_nWarnLogFile;
        }
        else
        {
            return std::cerr;
        }
    }
    else if (Error == level)
    {
        if (m_nErrLogFile.is_open())
        {
            return m_nErrLogFile;
        }
        else
        {
            return std::cerr;
        }
    }
    else
    {
        if (m_nFatalLogFile.is_open())
        {
            return m_nFatalLogFile;
        }
        else
        {
            return std::cerr;
        }
    }
}

std::ostream &Logger::log(log_level level,
                          const int line,
                          const std::string &function)
{

    if (level < Logger::m_nLevel)
        return Logger::m_nNullStream;

    time_t t = time(0);
    char tmpBuf[156];
    strftime(tmpBuf, 156, "%Y%m%d%H%M%S", localtime(&t)); //format date and time.

    std::string level_string = "";
    switch (level)
    {
    case Debug:
        level_string = "Debug";
        break;
    case Info:
        level_string = "Info";
        break;
    case Warning:
        level_string = "Warning";
        break;
    case Error:
        level_string = "Error";
        break;
    case Fatal:
        level_string = "Fatal";
        break;
    }

    //if the current log level is Info,so the LOG(Debug) do nothing
    return getStream(level) << "[" << tmpBuf << "]"
                            << "--"
                            << "[" << level_string << "]"
                            << "--"
                            << "function (" << function << ")"
                            << "line " << line << ":"
                            << std::flush;
}

Logger::~Logger()
{
    getStream(m_nLevel) << std::endl
                        << std::flush;

    if (Fatal == m_nLevel)
    {
        // m_nNullStream.close();

        m_nDebugLogFile.close();
        m_nInfoLogFile.close();
        m_nWarnLogFile.close();
        m_nErrLogFile.close();
        m_nFatalLogFile.close();
    }
    // std::cout << "class Logger destory\n";
}
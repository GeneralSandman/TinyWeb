#include "config.h"
#include "reader.h"
#include "log.h"

Configer::Configer(const std::string &config_file)
    : m_nFile(config_file),
      m_pFileReader(new Reader(config_file))
{

    init();
    LOG(Debug) << "class Configer constructor\n";
}

void Configer::init()
{
    m_nValue["listen"] = "";
    m_nValue["processpoll"] = "";
    m_nValue["docs"] = "";
    m_nValue["hostname"] = "";
    m_nValue["loglevel"] = "";
    m_nValue["logpath"] = "";
    m_nValue["debugfile"] = "";
    m_nValue["infofile"] = "";
    m_nValue["warnfile"] = "";
    m_nValue["errorfile"] = "";
    m_nValue["fatalfile"] = "";
}

void Configer::loadConfig()
{
}

Configer::~Configer()
{
    delete m_pFileReader;
    m_pFileReader = nullptr;
    LOG(Debug) << "class Configer constructor\n";
}
#include "ConfigFile.h"

const char host[] = "127.0.0.1";

ConfigFile::ConfigFile(const char* cfgFile)
: m_cfgFile(cfgFile)
{
}

ConfigFile::~ConfigFile()
{
}

const char* ConfigFile::GetIpAddress() const
{
    return host;
}

uint32_t ConfigFile::GetPort() const
{
    return 9090;
}

#ifndef __CONFIGFILE_H__
#define __CONFIGFILE_H__

#include <string>

class ConfigFile
{
public:
    ConfigFile (const char* cfgFile = nullptr);
    ~ConfigFile ();


    const char* GetIpAddress() const;
    uint32_t    GetPort() const;

private:
    std::string     m_cfgFile;
};

#endif

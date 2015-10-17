#ifndef __SERVERBASE_H__
#define __SERVERBASE_H__

#include "ConnectionManager.h"
#include "NetAcceptor.h"

using namespace Net;

class ConfigFile;
class NetSubscriber;

class ServerBase
{
public:
    ServerBase();
    ~ServerBase();

    int Init();
    int Fini();

    void Run();
private:
    ConfigFile*         m_cfg;
    NetAcceptor*        m_pAcceptor;
    ConnectionManager*  m_pConnMgr;
    NetSubscriber*      m_pSub;
};

#endif

#ifndef __SERVERBASE_H__
#define __SERVERBASE_H__

#include "ConnectionManager.h"
#include "NetAcceptor.h"

using namespace Net;

class ConfigFile;
class MessageManager;
class NetSubscriber;
class Timer;

class ServerBase
{
public:
    ServerBase();
    ~ServerBase();

    int Init();
    int Fini();

    void Run();
private:
    boost::asio::io_service io_service;
    ConfigFile*         m_cfg;
    MessageManager*     m_pMsgMgr;
    NetAcceptor*        m_pAcceptor;    // 侦听者
    ConnectionManager*  m_pConnMgr;
    Timer*              m_pTimer;
    NetSubscriber*      m_pSub;
};

#endif

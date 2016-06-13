#ifndef __SERVERBASE_H__
#define __SERVERBASE_H__

#include "ConnectionManager.h"
#include "NetAcceptor.h"
#include <memory>

using namespace Net;

class ConfigFile;
class MessageDispatcher;
class NetSubscriber;
class Timer;
class ChatroomManager;
class UserManager;

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
    std::shared_ptr<ConfigFile>         m_spcfg;
    std::shared_ptr<iPublisher>         m_spMsgMgr;
    std::shared_ptr<NetAcceptor>        m_spAcceptor;    // 侦听者
    std::shared_ptr<ConnectionManager>  m_spConnMgr;
    std::shared_ptr<Timer>              m_spTimer;
    std::shared_ptr<NetSubscriber>      m_spSub;
    std::shared_ptr<ChatroomManager>    m_spChatroomMgr;
    std::shared_ptr<UserManager>        m_spUserMgr;
};

#endif

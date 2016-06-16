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
class SessionManager;

class ServerBase
{
private:
    ServerBase();
    ServerBase(const ServerBase&) = delete;
    ServerBase(ServerBase&&) = delete;
    ServerBase& operator=(const ServerBase&) = delete;

public:
    ~ServerBase();

    static ServerBase* GetInstance();

    int Init();
    int Fini();

    std::shared_ptr<ConnectionManager>  GetConnMgr() { return m_spConnMgr; }
    std::shared_ptr<ChatroomManager>    GetChatroomMgr() { return m_spChatroomMgr; }
    std::shared_ptr<UserManager>        GetUserMgr() { return m_spUserMgr; }

    void Run();

private:
    static ServerBase*  m_sInstance;
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
    std::shared_ptr<SessionManager>     m_spSessionMgr;
};

#endif

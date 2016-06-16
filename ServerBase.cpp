#include "ServerBase.h"
#include "ConfigFile.h"
#include "NetSubscriber.h"
#include "Timer.h"
#include "MessageDispatcher.h"
#include "UserManager.h"
#include "ChatroomManager.h"
#include "SessionManager.h"

ServerBase* ServerBase::m_sInstance = nullptr;

ServerBase::ServerBase()
: m_spcfg(nullptr)
, m_spMsgMgr(nullptr)
, m_spAcceptor(nullptr)
, m_spConnMgr(nullptr)
, m_spTimer(nullptr)
, m_spSub(nullptr)
{
}

ServerBase::~ServerBase()
{
    Fini();
}

ServerBase* ServerBase::GetInstance()
{
    if (nullptr == m_sInstance) {
        m_sInstance = new ServerBase();
        m_sInstance->Init();
    }
    return m_sInstance;
}

int ServerBase::Init()
{
    m_spMsgMgr = std::shared_ptr<iPublisher>(new MessageDispatcher());

    m_spConnMgr = std::make_shared<ConnectionManager>(m_spMsgMgr);
    m_spConnMgr->Init();

    m_spAcceptor = std::make_shared<NetAcceptor>("127.0.0.1", 9095, io_service,
            m_spConnMgr);
    m_spAcceptor->Init();

    m_spTimer = std::make_shared<Timer>(io_service, m_spMsgMgr);
    m_spTimer->Init();

    m_spSub = std::make_shared<NetSubscriber>(m_spMsgMgr);
    m_spSub->Init();

    m_spChatroomMgr = std::shared_ptr<ChatroomManager>(new ChatroomManager(m_spMsgMgr));
    m_spChatroomMgr->Init();

    m_spUserMgr = std::shared_ptr<UserManager>(new UserManager(m_spMsgMgr));
    m_spUserMgr->Init();

    m_spSessionMgr = std::make_shared<SessionManager>(m_spMsgMgr);
    m_spSessionMgr->Init();

    return 0;
}

int ServerBase::Fini()
{
    return 0;
}

void ServerBase::Run()
{
    m_spTimer->Start();
    m_spAcceptor->Start();

    io_service.run();
}

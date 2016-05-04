#include "ServerBase.h"
#include "ConfigFile.h"
#include "NetSubscriber.h"
#include "Timer.h"
#include "MessageDispatcher.h"

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

int ServerBase::Init()
{
    //if (nullptr == m_cfg) {
    //    return -1;
    //}


    m_spMsgMgr = std::shared_ptr<iPublisher>(new MessageDispatcher());

    m_spConnMgr = std::make_shared<ConnectionManager>(m_spMsgMgr);
    m_spConnMgr->Init();

    m_spAcceptor = std::make_shared<NetAcceptor>("127.0.0.1", 9090, io_service, m_spConnMgr);
    m_spAcceptor->Init();

    m_spTimer = std::make_shared<Timer>(io_service, m_spMsgMgr);
    m_spTimer->Init();


    m_spSub = std::make_shared<NetSubscriber>(m_spMsgMgr);
    m_spSub->Init();

    return 0;
}

int ServerBase::Fini()
{
    //delete m_pSub;
    //delete m_pConnMgr;
    //delete m_pAcceptor;
    //delete m_pMsgMgr;
    //delete m_cfg;
    return 0;
}

void ServerBase::Run()
{
    m_spTimer->Start();
    m_spAcceptor->Start();

    io_service.run();
}

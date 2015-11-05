#include "ServerBase.h"
#include "ConfigFile.h"
#include "NetSubscriber.h"
#include "Timer.h"
#include "MessageManager.h"

ServerBase::ServerBase()
: m_cfg(nullptr)
, m_pMsgMgr(nullptr)
, m_pAcceptor(nullptr)
, m_pConnMgr(nullptr)
, m_pTimer(nullptr)
, m_pSub(nullptr)
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


    m_pMsgMgr = new MessageManager();

    m_pConnMgr = new ConnectionManager(m_pMsgMgr);
    m_pConnMgr->Init();

    m_pAcceptor = new NetAcceptor("127.0.0.1", 9090, io_service, m_pConnMgr);
    m_pAcceptor->Init();

    m_pTimer = new Timer(io_service, m_pMsgMgr);
    m_pTimer->Init();


    m_pSub = new NetSubscriber(m_pMsgMgr);
    m_pSub->Init();

    return 0;
}

int ServerBase::Fini()
{
    delete m_pSub;
    delete m_pConnMgr;
    delete m_pAcceptor;
    delete m_pMsgMgr;
    delete m_cfg;
}

void ServerBase::Run()
{
    m_pTimer->Start();
    m_pAcceptor->Start();

    io_service.run();
}

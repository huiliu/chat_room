#include "ServerBase.h"
#include "ConfigFile.h"
#include "NetSubscriber.h"

ServerBase::ServerBase()
: m_cfg(nullptr)
, m_pAcceptor(nullptr)
, m_pConnMgr(nullptr)
{
    Init();
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

    m_pConnMgr = new ConnectionManager();
    m_pSub = new NetSubscriber(m_pConnMgr);
    m_pSub->Init();

    return 0;
}

int ServerBase::Fini()
{
    delete m_pSub;
    delete m_pConnMgr;
    delete m_pAcceptor;
    delete m_cfg;
}

void ServerBase::Run()
{
    boost::asio::io_service io_service;
    m_pAcceptor = new NetAcceptor("127.0.0.1", 9090, io_service, m_pConnMgr);
    m_pAcceptor->Start();

    io_service.run();
}

#include "NetAcceptor.h"
#include "ConnectionManager.h"
#include <boost/bind.hpp>
#include <iostream>
#include <cassert>

namespace Net
{

NetAcceptor::NetAcceptor(const char* ipaddr, uint32_t port, boost::asio::io_service& service, ConnectionManager* pConnMgr)
: m_pAcceptor(service, ip::tcp::endpoint(ip::address().from_string(ipaddr), port))
, m_pConnMgr(pConnMgr)
{
    m_IpAddress.from_string(ipaddr);
    Init();
}

NetAcceptor::~NetAcceptor()
{
    Fini();
}

int NetAcceptor::Init()
{
    return 0;
}

int NetAcceptor::Fini()
{
    return 0;
}

void NetAcceptor::Start()
{
    if (nullptr == m_pConnMgr) {
        assert(false);
        return;
    }
    NetConnection* pConn = m_pConnMgr->CreateConnection(m_pAcceptor.get_io_service());
    m_pAcceptor.async_accept(pConn->socket(),
        boost::bind(&NetAcceptor::AcceptHandler, this,
            pConn->GetConnId(), 
            boost::asio::placeholders::error));

}

void NetAcceptor::AcceptHandler(ConnID cId, const boost::system::error_code& err)
{
    NetConnection* pConn = m_pConnMgr->FindConnection(cId);
    if (!err && nullptr != pConn) {
        pConn->Start();

        Start();
    }
    else
    {
        m_pConnMgr->RemoveConnection(cId);
    }
}


}

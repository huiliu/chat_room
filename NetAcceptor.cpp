#include "NetAcceptor.h"
#include "ConnectionManager.h"
#include <boost/bind.hpp>
#include <iostream>
#include <cassert>
#include "CommonApi.h"

namespace Net
{

NetAcceptor::NetAcceptor(const char* ipaddr, uint32_t port, boost::asio::io_service& service, std::shared_ptr<ConnectionManager> pConnMgr)
: m_pAcceptor(service, ip::tcp::endpoint(ip::address().from_string(ipaddr), port))
, m_pConnMgr(pConnMgr)
{
    m_IpAddress.from_string(ipaddr);
}

NetAcceptor::~NetAcceptor()
{
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
    std::shared_ptr<NetConnection> spConn = m_pConnMgr->CreateConnection(m_pAcceptor.get_io_service());
    m_pAcceptor.async_accept(spConn->socket(),
        boost::bind(&NetAcceptor::AcceptHandler, this,
            spConn->GetConnId(), 
            boost::asio::placeholders::error));

}

void NetAcceptor::AcceptHandler(ConnID cId, const boost::system::error_code& err)
{
    std::shared_ptr<NetConnection> spConn = m_pConnMgr->FindConnection(cId);
    if (!err && nullptr != spConn) {
        spConn->Start();
        DEBUG_LOG("有新连接进入!");
    }
    else
    {
        m_pConnMgr->RemoveConnection(cId);
    }

    Start();
}


}

#include "NetConnector.h"
#include "ServerBase.h"
#include "iCryptTool.h"
#include "ConnectionManager.h"
#include "NetConnection.h"
#include "src/MessageID.pb.h"
#include <boost/bind.hpp>

// 发起socket连接
//

namespace Net
{

NetConnector::NetConnector(const std::string& host, const std::string& service, std::shared_ptr<ConnectionManager>& spConnMgr)
    : m_ServerName(host)
    , m_ServicePort(service)
    , m_spConnMgr(spConnMgr)
{
}

NetConnector::~NetConnector()
{
}

int NetConnector::Init()
{
    return 0;
}

int NetConnector::Fini()
{
    return 0;
}

int NetConnector::Start()
{
    int retval = 1;
    m_spConn = m_spConnMgr->CreateConnection(
            ServerBase::GetInstance()->GetIoService()
            );
    if (m_spConn) {
        ip::tcp::resolver::query query(m_ServerName, m_ServicePort);
        ip::tcp::resolver resolver(ServerBase::GetInstance()->GetIoService());
        ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        // TODO: 如何判断connect成功与否
        boost::asio::connect(m_spConn->socket(), endpoint_iterator);
        
        retval = 0;
    }

    return retval;
}

void NetConnector::SendPacket(std::shared_ptr<RawMessage> spMsg)
{
    m_spConn->SendPacket(spMsg);
}
}

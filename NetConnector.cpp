#include "NetConnector.h"
#include "iCryptTool.h"
#include "src/MessageID.pb.h"
#include <boost/bind.hpp>

// 发起socket连接
//

namespace Net
{

NetConnector::NetConnector(io_service& io)
    : m_Socket(io)
    , m_Resolver(io)
{
}

NetConnector::~NetConnector()
{
    m_Socket.close();
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
    ip::tcp::resolver::query query("127.0.0.1", "9095");
    ip::tcp::resolver::iterator endpoint_iterator = m_Resolver.resolve(query);

    // TODO: 如何判断connect成功与否
    boost::asio::connect(m_Socket, endpoint_iterator);

    boost::asio::async_read(m_Socket, boost::asio::buffer(m_readBuff), 
        boost::asio::transfer_at_least(1),
        boost::bind(&NetConnector::AsyncReadHandler, this, 
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

    return 0;
}

void NetConnector::AsyncReadHandler(const boost::system::error_code& err,
    size_t byte_transferred)
{
    if (!err) {
        return;
    }

    boost::asio::async_read(m_Socket, boost::asio::buffer(m_readBuff), 
        boost::asio::transfer_at_least(1),
        boost::bind(&NetConnector::AsyncReadHandler, this, 
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void NetConnector::AsyncWriteHandler(const boost::system::error_code& err,
    size_t byte_transferred)
{
    if (!err) {
        // 发送失败 
    }
    else
    {
        // 发送成功
    }
}

void NetConnector::SendPacket(std::shared_ptr<RawMessage> spMsg)
{
    spMsg->SerializeToString(&m_writeBuff);
    
    boost::asio::async_write(m_Socket, boost::asio::buffer(m_writeBuff),
        boost::bind(&NetConnector::AsyncWriteHandler, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
}

void NetConnector::Encrypt(char* pData, uint32_t sz)
{
    m_spCryTool->Encrypt(pData, sz);
}

void NetConnector::Decrypt(char* pData, uint32_t sz)
{
    m_spCryTool->Decrypt(pData, sz);
}

}

#include "NetConnection.h"
#include <boost/bind.hpp>
#include "ConnectionManager.h"
#include <iostream>
#include "src/MessageID.pb.h"

namespace Net
{

NetConnection::NetConnection(io_service& ioservice, ConnectionManager* pConnMgr)
: m_connId(0)
, m_socket(ip::tcp::socket(ioservice))
, m_pConnMgr(pConnMgr)
{
}

NetConnection::~NetConnection()
{
}

ip::tcp::socket& NetConnection::socket()
{
    return m_socket;
}

void NetConnection::Start()
{
    boost::asio::async_read(m_socket, boost::asio::buffer(m_readBuff), boost::asio::transfer_at_least(10),
        boost::bind(&NetConnection::AsyncReadHandler, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void NetConnection::SendPacket(std::shared_ptr<RawMessage> pMsg)
{
    pMsg->SerializeToString(&m_writeBuff);
    boost::asio::async_write(m_socket,
                            boost::asio::buffer(m_writeBuff),
                            boost::bind(&NetConnection::AsyncWriteHandler, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void NetConnection::AsyncReadHandler(const boost::system::error_code& err, size_t byte_transferred)
{
    if (!err) {
        auto spMsg = std::make_shared<RawMessage>();
        spMsg->ParseFromString(m_readBuff);
        m_pConnMgr->PutInRecvQueue(m_connId, spMsg);

        Start();
    }
    else if (boost::asio::error::eof == err) {
        // 对方关闭了连接
        Close();
    }
    else
    {
        std::cout << err << std::endl;
        Close();
    }
}

void NetConnection::AsyncWriteHandler(const boost::system::error_code& err, size_t byte_transferred)
{
    if (!err) {
        // 发送成功
    }
    else
    {
        // 发送失败
    }
}

}

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
, m_bFirstPacket(true)
, m_bSentCryptKey(false)
, m_bVersionPassed(false)
, m_bAccountPassed(false)
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
    // 必须保证回调函数执行完之前，m_readBuff是有效的
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
        // bool ParseFromString(const string& data)
        if (spMsg->ParseFromString(m_readBuff)) {
            if (m_bFirstPacket) {
                m_bFirstPacket = false;

                SendCryptKey();
                SendVersion();
            }
            else if (!m_bVersionPassed &&
                                spMsg->id() == MSG_CHECK_VERSION_RESULT) {
                if (CheckVersion()) {
                    m_bVersionPassed = true;
                    CheckAccount();
                }
            }
            else if (m_bAccountPassed) {
                m_pConnMgr->PutInRecvQueue(m_connId, spMsg);
            }
        }

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

void NetConnection::SendCryptKey()
{

}

void NetConnection::SendVersion()
{
}

bool NetConnection::CheckVersion()
{
}

bool NetConnection::CheckAccount()
{
}

}

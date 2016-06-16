#include "NetConnection.h"
#include <boost/bind.hpp>
#include "ConnectionManager.h"
#include <iostream>
#include "src/MessageID.pb.h"
#include "src/version.pb.h"
#include "iCryptTool.h"
#include "CommonApi.h"

namespace Net
{

NetConnection::NetConnection(io_service& ioservice, std::shared_ptr<ConnectionManager> spConnMgr)
: m_connId(0)
, m_socket(ip::tcp::socket(ioservice))
, m_spConnMgr(spConnMgr)
, m_bFirstPacket(true)
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
    if (m_bFirstPacket) {
        m_bFirstPacket = false;
        SendCryptKey();

        SendVersion();
    }
    // 必须保证回调函数执行完之前，m_readBuff是有效的
    boost::asio::async_read(m_socket, boost::asio::buffer(m_readBuff), boost::asio::transfer_at_least(10),
        boost::bind(&NetConnection::AsyncReadHandler, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void NetConnection::SendPacket(std::shared_ptr<RawMessage> pMsg)
{
    pMsg->SerializeToString(&m_writeBuff);
    //Encrypt(m_writeBuff.c_str(), m_writeBuff.size());
    boost::asio::async_write(m_socket,
                            boost::asio::buffer(m_writeBuff),
                            boost::bind(&NetConnection::AsyncWriteHandler, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void NetConnection::AsyncReadHandler(const boost::system::error_code& err, size_t byte_transferred)
{
    if (!err) {

        Decrypt(m_readBuff, byte_transferred);

        auto spMsg = std::make_shared<RawMessage>();
        spMsg->set_clientid(GetConnId());
        // TODO: 
        // 1. 如果protobuf有字段为0，接收到的数据中会有'\0'存在，
        // 而在C/C++的字符数组中'\0'表示结尾，导致ParseFromString没有读取到所
        // 有的数据
        // 2. ParseFromString是如何处理内存的。
        if (spMsg->ParseFromString(m_readBuff)) {
            m_spConnMgr->PutInRecvQueue(m_connId, spMsg);    
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
    std::shared_ptr<char> spKeyBuff = GenerateRandomChar();
    m_spCryTool = CryptFactory(CT_NULL, spKeyBuff, g_key_buff_len);

    boost::system::error_code ec;
    // TODO: 此处需要调整
    uint32_t sz = boost::asio::write(m_socket,
            boost::asio::buffer(std::string(spKeyBuff.get())), ec);

    if (g_key_buff_len != sz) {
        assert("发送密钥失败！");
    }
    DEBUG_LOG("密钥已发送！");
}

void NetConnection::SendVersion()
{
    ReqCheckVersion req;
    std::shared_ptr<RawMessage> spRaw(new RawMessage());
    spRaw->set_id(req.msg_id());
    spRaw->set_strmsg(req.SerializeAsString());

    m_spConnMgr->PutInSendQueue(m_connId, spRaw);
    DEBUG_LOG("已发送版本号!");
}

void NetConnection::Encrypt(char* pData, uint32_t sz)
{
    if (m_spCryTool) {
        m_spCryTool->Encrypt(pData, sz);    
    }
}

void NetConnection::Decrypt(char* pData, uint32_t sz)
{
    if (m_spCryTool) {
        m_spCryTool->Decrypt(pData, sz);
    }
}

}

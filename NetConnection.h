#ifndef __NETCONNECTION_H__
#define __NETCONNECTION_H__
//
// 表示一条socket连接，接收/发送消息
// 
#include <memory>
#include <string>
#include "CommonDataType.h"

class RawMessage;
namespace Net{

using namespace boost::asio;

const   uint32_t RECEVIER_BUFFER_SIZE = 8196; // 接收网络数据缓冲区大小

class ConnectionManager;

class NetConnection
{
public:
    NetConnection (io_service& Ioservice, ConnectionManager* pConnMgr);
    ~NetConnection ();

    int Init();
    int Fini();

    ip::tcp::socket& socket();
    void Start();
    void Close() { m_socket.close(); }

    void    SetConnId(ConnID cid) { m_connId = cid; }
    ConnID  GetConnId() const { return m_connId; }

    void AsyncReadHandler(const boost::system::error_code& err, size_t byte_transferred);
    void AsyncWriteHandler(const boost::system::error_code& err, size_t byte_transferred);

    void SendPacket(std::shared_ptr<RawMessage> pMsg);

private:
    void    SendCryptKey();     // 发送密钥
    void    SendVersion();      // 发送协议号
    bool    CheckVersion();     // 检查版本
    bool    CheckAccount();     // 检查帐号，密码是否通过验证

private:
    ConnID                      m_connId;
    ip::tcp::socket             m_socket;
    ConnectionManager*          m_pConnMgr;
    char                        m_readBuff[RECEVIER_BUFFER_SIZE];
    std::string                 m_writeBuff;

    bool                        m_bFirstPacket;     // 第一个网络包
    bool                        m_bSentCryptKey;    // 是否已经发送密钥
    bool                        m_bVersionPassed;   // 版本检查是否通过
    bool                        m_bAccountPassed;   // 帐号验证是否已经通过
};

}
#endif

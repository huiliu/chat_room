#ifndef __NETCONNECTION_H__
#define __NETCONNECTION_H__
//
// 表示一条socket连接，接收/发送消息
// 
#include <memory>
#include <string>
#include "CommonDataType.h"

using namespace boost::asio;
class RawMessage;
class iCryptTool;

namespace Net{


const   uint32_t RECEVIER_BUFFER_SIZE = 8196; // 接收网络数据缓冲区大小

class ConnectionManager;

class NetConnection
{
public:
    NetConnection (io_service& Ioservice, std::shared_ptr<ConnectionManager> spConnMgr);
    ~NetConnection ();

    int Init();
    int Fini();

    ip::tcp::socket& socket();
    void Start();
    void Close() { m_socket.close(); }

    void    SetConnId(ConnID cid) { m_connId = cid; }
    ConnID  GetConnId() const { return m_connId; }

    void SendPacket(std::shared_ptr<RawMessage> pMsg);

private:
    void AsyncReadHandler(const boost::system::error_code& err, size_t byte_transferred);
    void AsyncWriteHandler(const boost::system::error_code& err, size_t byte_transferred);

    void    Encrypt(char* pData, uint32_t sz);
    void    Decrypt(char* pData, uint32_t sz);

    void    SendCryptKey();     // 发送密钥
    void    SendVersion();      // 发送协议号

private:
    ConnID                      m_connId;
    ip::tcp::socket             m_socket;
    std::shared_ptr<ConnectionManager>          m_spConnMgr;
    char                        m_readBuff[RECEVIER_BUFFER_SIZE];
    std::string                 m_writeBuff;
    std::shared_ptr<iCryptTool> m_spCryTool;

    bool                        m_bFirstPacket;     // 第一个网络包
};

}
#endif

#ifndef __NETCONNECTION_H__
#define __NETCONNECTION_H__
#include <memory>
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
    ConnID                      m_connId;
    ip::tcp::socket             m_socket;
    char                        m_buff[RECEVIER_BUFFER_SIZE];
    ConnectionManager*          m_pConnMgr;
};

}
#endif

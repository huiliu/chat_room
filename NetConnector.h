#ifndef __CHATROOM_NETCONNECTOR_H__
#define __CHATROOM_NETCONNECTOR_H__ 1
//
// 用于发起socket连接
//
#include <boost/asio.hpp>
#include <memory>
#include <string>

using namespace boost::asio;

class RawMessage;
class iCryptTool;

namespace Net
{
class NetConnector
{
public:
    NetConnector(io_service& io);
    ~NetConnector ();

    NetConnector(const NetConnector&) = delete;
    NetConnector(NetConnector&&)      = delete;
    NetConnector& operator= (const NetConnector&) = delete;

    int Init();
    int Fini();

    int Start();

    void AsyncReadHandler(const boost::system::error_code& err, size_t bytes_transferred);
    void AsyncWriteHandler(const boost::system::error_code& err, size_t bytes_transferred);

    void SendPacket(std::shared_ptr<RawMessage> spMsg);

private:
    void Encrypt(char* pData, uint32_t sz);
    void Decrypt(char* pData, uint32_t sz);

private:
    ip::tcp::socket                 m_Socket;
    ip::tcp::resolver               m_Resolver;
    std::shared_ptr<iCryptTool>     m_spCryTool;

    char                            m_readBuff[8096];
    std::string                     m_writeBuff;
};
}
#endif /* ifndef __CHATROOM_NETCONNECTOR_H__ */

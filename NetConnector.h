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

class NetConnection;
class ConnectionManager;

class NetConnector
{
public:
    NetConnector(const std::string& host, const std::string& service, std::shared_ptr<ConnectionManager>& spConnMgr);
    ~NetConnector ();

    NetConnector(const NetConnector&) = delete;
    NetConnector(NetConnector&&)      = delete;
    NetConnector& operator= (const NetConnector&) = delete;

    int Init();
    int Fini();

    int Start();
    void SendPacket(std::shared_ptr<RawMessage> spMsg);

private:
    std::string                     m_ServerName;
    std::string                     m_ServicePort;
    std::shared_ptr<ConnectionManager>  m_spConnMgr;
    std::shared_ptr<NetConnection>  m_spConn;
    std::shared_ptr<iCryptTool>     m_spCryTool;

    char                            m_readBuff[8096];
    std::string                     m_writeBuff;
};


}
#endif /* ifndef __CHATROOM_NETCONNECTOR_H__ */

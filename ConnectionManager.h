#ifndef __CONNECTIONMANAGER_H__
#define __CONNECTIONMANAGER_H__

#include <map>
#include <boost/asio.hpp>
#include "src/ConnectionInfo.pb.h"
#include "NetConnection.h"
#include "iPublisher.h"

namespace Net {

using namespace boost::asio::ip;

class NetConnection;
class ConnInfo;

class ConnectionManager : public iPublisher
{
public:
    ConnectionManager();
    ~ConnectionManager();

    void Init();
    void Fini();

    NetConnection* CreateConnection(boost::asio::io_service& io_service);
    NetConnection* FindConnection(ConnID cId);

    void RemoveConnection(ConnID cid);

    void UpdateConnStatus(uint64_t, CONN_STATUS status);

    bool IsConnected(uint64_t uid);

private:
    typedef std::map<uint64_t, NetConnection*> MAP_CONN;

    MAP_CONN    m_mapConn;
    uint64_t    m_connId;
};

}
#endif

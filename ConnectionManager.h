#ifndef __CONNECTIONMANAGER_H__
#define __CONNECTIONMANAGER_H__

//
// 管理客户端连接
// 分发接收到的网络消息
//
//

#include <map>
#include <queue>
#include <memory>
#include <boost/asio.hpp>
#include "src/ConnectionInfo.pb.h"
#include "NetConnection.h"
#include "iSubscriber.h"
#include "CommonDataType.h"

using namespace boost::asio::ip;

namespace Net {

class NetAcceptor;
class NetConnection;
class ConnInfo;


class ConnectionManager : public iSubscriber , public std::enable_shared_from_this<ConnectionManager>
{
public:
    ConnectionManager(std::shared_ptr<iPublisher> spMgr);
    ~ConnectionManager();

    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager(ConnectionManager&&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    int Init();
    int Fini();

    virtual void HandleMessage(SpRawMessage spMsg);

    std::shared_ptr<NetConnection> CreateConnection(boost::asio::io_service& io_service);
    std::shared_ptr<NetConnection> FindConnection(ConnID cId);
    size_t                          GetConnectionCount() const { return m_mapConn.size(); }

    void RemoveConnection(ConnID cid);
    void UpdateConnStatus(uint64_t, CONN_STATUS status);
    bool IsConnected(uint64_t uid);

    // 将消息压入发送缓冲队列
    void PutInSendQueue(ConnID connId, std::shared_ptr<RawMessage> spMsg);

    // 将消息压入接收缓冲队列
    void PutInRecvQueue(ConnID connId, std::shared_ptr<RawMessage> spMsg);
    // 读取一条接收队列中的网络消息
    ConnID GetMessage(std::shared_ptr<RawMessage> spMsg);

private:
    // 将发送缓冲队列中的待发送消息经网络发送
    void FlushSendQueue();
    // 将接收到的网络消息广播
    void FlushRecvQueue();
    // 处理时间脉冲消息
    void HandleSystemTimeTick(SpRawMessage spMsg);

private:
    typedef std::map<ConnID, std::shared_ptr<NetConnection>> MAP_CONN;

    std::shared_ptr<iPublisher> m_spMgr;
    MAP_CONN    m_mapConn;
    uint64_t    m_connId;

    std::deque<NetMessageEntity>  m_queueRecv;  // 消息接收队列
    std::deque<NetMessageEntity>  m_queueSend;  // 消息发送队列
};

}
#endif

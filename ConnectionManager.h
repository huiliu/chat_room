#ifndef __CONNECTIONMANAGER_H__
#define __CONNECTIONMANAGER_H__

#include <map>
#include <queue>
#include <memory>
#include <boost/asio.hpp>
#include "src/ConnectionInfo.pb.h"
#include "NetConnection.h"
#include "iSubscriber.h"
#include "CommonDataType.h"

namespace Net {

using namespace boost::asio::ip;

class NetAcceptor;
class NetConnection;
class ConnInfo;

/*
 * 管理客户端连接
 * 分发接收到的网络消息
 *
 */

class ConnectionManager : public iSubscriber
{
public:
    ConnectionManager(iPublisher* pMgr/*, boost::asio::io_service& io_service*/);
    ~ConnectionManager();

    void Init();
    void Fini();

    virtual void HandleMessage(SpRawMessage spMsg);

    NetConnection* CreateConnection(boost::asio::io_service& io_service);
    NetConnection* FindConnection(ConnID cId);

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
    typedef std::map<ConnID, NetConnection*> MAP_CONN;

    iPublisher* m_pMgr;
    MAP_CONN    m_mapConn;
    uint64_t    m_connId;

    std::deque<NetMessageEntity>  m_queueRecv;  // 消息接收队列
    std::deque<NetMessageEntity>  m_queueSend;  // 消息发送队列

    boost::asio::io_service     m_ioService;
    NetAcceptor*                m_pAcceptor;    // 侦听者
};

}
#endif

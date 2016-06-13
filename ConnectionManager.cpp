#include "ConnectionManager.h"
#include "NetConnection.h"
#include "iPublisher.h"
#include "NetAcceptor.h"


namespace Net
{

ConnectionManager::ConnectionManager(std::shared_ptr<iPublisher> spMgr)
: m_spMgr(spMgr)
, m_mapConn()
, m_connId(0)
, m_queueRecv()
, m_queueSend()
{
}

ConnectionManager::~ConnectionManager()
{
    Fini();
}

void ConnectionManager::Init()
{
    m_spMgr->Register(MSG_TIME_TICK, this);
}

void ConnectionManager::Fini()
{
    m_spMgr->Unregister(MSG_TIME_TICK, this);

    m_mapConn.clear();
}

void ConnectionManager::HandleMessage(SpRawMessage spMsg)
{
    BOOST_ASSERT(MSG_TIME_TICK == spMsg->id());

    switch(spMsg->id())
    {
    case MSG_TIME_TICK:
        HandleSystemTimeTick(spMsg);
        break;
    default:
        BOOST_ASSERT(false);
    }
}


void ConnectionManager::HandleSystemTimeTick(SpRawMessage /* spMsg */)
{
    // 根据系统时间脉冲来冲刷发送队列
    FlushSendQueue();
    FlushRecvQueue();
}


void ConnectionManager::RemoveConnection(ConnID cid)
{
    MAP_CONN::iterator it = m_mapConn.find(cid);
    if (m_mapConn.end() != it) {
        it->second->Close();
        m_mapConn.erase(it);
    }
}

std::shared_ptr<NetConnection> ConnectionManager::FindConnection(ConnID cId)
{
    std::shared_ptr<NetConnection> retVal = nullptr;
    MAP_CONN::iterator it = m_mapConn.find(cId);

    if (m_mapConn.end() != it) {
        retVal = it->second;
    }
    return retVal;
}

void ConnectionManager::UpdateConnStatus(uint64_t uid, CONN_STATUS status)
{
    /*
    MAP_CONN::iterator it = m_mapConn.find(uid);
    if (m_mapConn.end() != it) {
        ConnInfo* conn = it->second;
        conn->set_status(status);
    }
    */
}

std::shared_ptr<NetConnection> ConnectionManager::CreateConnection(boost::asio::io_service& io_service)
{
    ++m_connId;
    std::shared_ptr<NetConnection> spConn = std::make_shared<NetConnection>(io_service, shared_from_this()); 
    spConn->SetConnId(m_connId);

    m_mapConn.insert(std::make_pair(m_connId, spConn));
    return spConn;
}

void ConnectionManager::PutInSendQueue(ConnID connId, std::shared_ptr<RawMessage> spMsg)
{
    m_queueSend.push_back(std::make_pair(connId, spMsg));
}

void ConnectionManager::FlushSendQueue()
{
    // Maybe 需要一些控制
    while (!m_queueSend.empty()) {
        NetMessageEntity& item = m_queueSend.front();
        std::shared_ptr<NetConnection> spConn = FindConnection(item.first);
        BOOST_ASSERT(nullptr != spConn);
        if (nullptr != spConn) {
            spConn->SendPacket(item.second);
        }
        m_queueSend.pop_front();
    }
}

void ConnectionManager::PutInRecvQueue(ConnID connId, std::shared_ptr<RawMessage> spMsg)
{
    m_queueRecv.emplace_back(connId, spMsg);
}

ConnID ConnectionManager::GetMessage(std::shared_ptr<RawMessage> spMsg)
{
    NetMessageEntity& item = m_queueRecv.front();
    ConnID cid = item.first;
    spMsg = item.second;

    m_queueRecv.pop_front();
    return cid;
}

void ConnectionManager::FlushRecvQueue()
{
    while (!m_queueRecv.empty()) {
        NetMessageEntity& msg = m_queueRecv.front();
        m_spMgr->Notify(msg.second);

        m_queueRecv.pop_front();
    }
}

}

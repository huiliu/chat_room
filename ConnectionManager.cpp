#include "ConnectionManager.h"
#include "NetConnection.h"
#include "iPublisher.h"
#include "NetAcceptor.h"


namespace Net
{

ConnectionManager::ConnectionManager(iPublisher* pMgr/*, boost::asio::io_service& io_service*/)
: m_pMgr(pMgr)
, m_mapConn()
, m_connId(0)
, m_queueRecv()
, m_queueSend()
, m_ioService()
, m_pAcceptor(nullptr)
{
}

ConnectionManager::~ConnectionManager()
{
    Fini();
}

void ConnectionManager::Init()
{
    m_pMgr->Attach(MSG_TIME_TICK, this);
}

void ConnectionManager::Fini()
{
    m_pMgr->Detach(MSG_TIME_TICK, this);


    for (auto& it : m_mapConn) {
        delete it.second;
    }
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

NetConnection* ConnectionManager::FindConnection(ConnID cId)
{
    NetConnection* retVal = nullptr;
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

NetConnection* ConnectionManager::CreateConnection(boost::asio::io_service& io_service)
{
    ++m_connId;
    NetConnection* pConn = new NetConnection(io_service, this); 
    pConn->SetConnId(m_connId);

    m_mapConn.insert(std::make_pair(m_connId, pConn));
    return pConn;
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
        NetConnection* pConn = FindConnection(item.first);
        BOOST_ASSERT(nullptr != pConn);
        if (nullptr != pConn) {
            pConn->SendPacket(item.second);
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
        m_pMgr->Notify(msg.second);

        m_queueRecv.pop_front();
    }
}

}

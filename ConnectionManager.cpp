#include "ConnectionManager.h"
#include "NetConnection.h"

namespace Net
{

ConnectionManager::ConnectionManager()
: m_mapConn()
, m_connId(0)
{
    Init();
}

ConnectionManager::~ConnectionManager()
{
    Fini();
}

void ConnectionManager::Init()
{
}

void ConnectionManager::Fini()
{
    m_mapConn.clear();
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

    m_mapConn[m_connId] = pConn;
    return pConn;
}

}

#ifndef __NETACCEPTOR_H__
#define __NETACCEPTOR_H__
#include <boost/asio.hpp>
#include <string>
#include "NetConnection.h"

namespace Net{

using namespace boost::asio;

class ConnectionManager;
class NetConnection;


class NetAcceptor
{
public:
    NetAcceptor (const char* ip, uint32_t port, boost::asio::io_service& service, ConnectionManager* pConnMgr);
    ~NetAcceptor ();

    int Init();
    int Fini();

    void Start();

private:
    void AcceptHandler(ConnID cId, const boost::system::error_code& err);

private:
    ip::address         m_IpAddress;
    uint32_t            m_Port;
    ip::tcp::acceptor   m_pAcceptor;
    ConnectionManager*  m_pConnMgr;
};

}
#endif

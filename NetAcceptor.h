/*
 * author: LiuHui <hui.liu.e@gmail.com>
 */
#ifndef __NETACCEPTOR_H__
#define __NETACCEPTOR_H__
#include <boost/asio.hpp>
#include <string>
#include "NetConnection.h"

using namespace boost::asio;

namespace Net{

class ConnectionManager;
class NetConnection;

/*
 * 侦听端口，等待客户端的连接
 */

class NetAcceptor
{
public:
    NetAcceptor (const char* ip, uint32_t port, boost::asio::io_service& service, std::shared_ptr<ConnectionManager> pConnMgr);
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
    std::shared_ptr<ConnectionManager>  m_pConnMgr;
};

}
#endif

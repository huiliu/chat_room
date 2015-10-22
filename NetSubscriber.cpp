#include <iostream>
#include "NetSubscriber.h"
#include "NetPublisher.h"
#include "src/MessageID.pb.h"
#include "src/Login.pb.h"

NetSubscriber::NetSubscriber(iPublisher* pPub)
: m_pPublisher(pPub)
{

}

NetSubscriber::~NetSubscriber()
{
    Fini();
}

void NetSubscriber::Init()
{
    m_pPublisher->Attach(MSG_REQ_LOGIN, this);
    m_pPublisher->Attach(MSG_TIME_TICK, this);
}

void NetSubscriber::Fini()
{
    m_pPublisher->Detach(MSG_TIME_TICK, this);
    m_pPublisher->Detach(MSG_REQ_LOGIN, this);
}

void NetSubscriber::HandleMessage(std::shared_ptr<RawMessage> pMsg)
{
    switch (pMsg->id()) {
        case MSG_REQ_LOGIN:
            HandleReqLogin(pMsg);
            break;
        case MSG_TIME_TICK:
            HandleSystemTick(pMsg);
            break;
        default:
            break;
    }
}

void NetSubscriber::HandleReqLogin(std::shared_ptr<RawMessage> pMsg)
{
    ReqLogin req;
    req.ParseFromString(pMsg->strmsg());

    std::cout << req.username() << "\t" << req.password() << std::endl;
}

void NetSubscriber::HandleSystemTick(std::shared_ptr<RawMessage> pMsg)
{
    std::cout << "System Time Tick!" << std::endl;
}

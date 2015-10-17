#include "NetSubscriber.h"
#include "src/MessageID.pb.h"
#include "NetPublisher.h"
#include <iostream>

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
}

void NetSubscriber::Fini()
{
    m_pPublisher->Detach(MSG_REQ_LOGIN, this);
}

void NetSubscriber::HandleMessage(std::shared_ptr<RawMessage> pMsg)
{
    switch (pMsg->id()) {
        case MSG_REQ_LOGIN:
            {
                HandleReqLogin(pMsg);
            }            
            break;
        default:
            break;
    }
}

void NetSubscriber::HandleReqLogin(std::shared_ptr<RawMessage> pMsg)
{
    std::cout << "New User Request Login!" << std::endl;
}

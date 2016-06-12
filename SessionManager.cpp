#include "SessionManager.h"
#include "iPublisher.h"

SessionManager::SessionManager(std::shared_ptr<iPublisher> spPublisher)
    : m_spPublisher(spPublisher)
{
}

SessionManager::~SessionManager()
{
}

int SessionManager::Init()
{
    m_spPublisher->Register(MSG_REQ_LOGIN, this);
    return 0;
}

int SessionManager::Fini()
{
    m_spPublisher->Unregister(MSG_REQ_LOGIN, this);
    return 0;
}

void SessionManager::HandleMessage(std::shared_ptr<RawMessage> spMsg)
{
    switch (spMsg->id()) {
        case MSG_REQ_LOGIN:
            HandleReqLogin(spMsg);
            break;
        default:
            assert(false);
    }
}

void SessionManager::HandleReqLogin(std::shared_ptr<RawMessage> spMsg)
{

}

void SessionManager::HandleLogout(std::shared_ptr<RawMessage> spMsg)
{
}

int SessionManager::OnLogin(uint32_t uid)
{
    return 0;
}

int SessionManager::OnLogout(uint32_t uid)
{
    return 0;
}

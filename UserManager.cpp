#include "UserManager.h"

UserManager::UserManager(std::shared_ptr<iPublisher> spPubliser)
    : m_spPublisher(spPubliser)
{
}

UserManager::~UserManager()
{
}

int UserManager::Init()
{
    return 0;
}

int UserManager::Fini()
{
    return 0;
}

void UserManager::HandleMessage(std::shared_ptr<RawMessage> spMsg)
{
    switch (spMsg->id()) {
        case MSG_TIME_TICK:
            
            break;
        default:
            assert(false);
    }
}

UserData& UserManager::FindUser(uint32_t uid)
{
    UserData data;

    return data;
}

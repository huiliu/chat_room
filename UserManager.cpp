#include "UserManager.h"
#include "User.h"

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

std::shared_ptr<User> UserManager::CreateUser(const UserData& data)
{
    std::shared_ptr<User> spUser = std::make_shared<User>(data);
    m_mapUserData.emplace(spUser->GetUserId(), spUser);

    return spUser;
}

std::shared_ptr<User> UserManager::FindUser(uint32_t uid)
{
    UserMapTypeIterator it = m_mapUserData.find(uid);
    if (m_mapUserData.end() != it) {
        return it->second;
    }
    return nullptr;
}

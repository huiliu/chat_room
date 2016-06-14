#include "User.h"

User::User(const UserData& data)
    : m_UserData(data)
    , m_ConnId(0)
{
}

User::~User()
{
}

void User::SetUserName(const std::string& name)
{
    m_UserData.set_name(name);
}

void User::SetUserId(uint32_t id)
{
    m_UserData.set_id(id);
}

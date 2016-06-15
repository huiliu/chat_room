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

void User::SetPriority(uint32_t p)
{
    m_UserData.set_priority(p);
}

uint32_t User::GetPriority() const
{
    return m_UserData.priority();
}

void User::SetChatroomId(uint32_t cid)
{
    m_UserData.set_chatroom_id(cid);
}

uint32_t User::GetChatroomId() const
{
    return m_UserData.chatroom_id();
}

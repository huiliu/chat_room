#include "Chatroom.h"
#include <algorithm>

Chatroom::Chatroom(const CHATROOM_DATA& data)
    : m_BasicData(data)
{
}

Chatroom::~Chatroom()
{
}

void Chatroom::SetChatroomId(uint32_t uid)
{
    m_BasicData.set_id(uid);
}

uint32_t  Chatroom::GetChatroomId() const
{
    return m_BasicData.id();
}

void Chatroom::SetChatroomName(const std::string& name)
{
    m_BasicData.set_name(name);
}

const std::string& Chatroom::GetChatroomName() const
{
    return m_BasicData.name();
}

void Chatroom::SetChatroomNotice(const std::string& notice)
{
    m_BasicData.set_notice(notice);
}

const std::string& Chatroom::GetChatroomNotice() const
{
    return m_BasicData.notice();
}

bool Chatroom::IsInChatroom(uint32_t uid)
{
    auto it = std::find(m_VecUser.begin(), m_VecUser.end(), uid);
    if (m_VecUser.end() != it) {
        return true;
    }
    return false;
}

void Chatroom::AddUser(uint32_t uid)
{
    if (!IsInChatroom(uid)) {
        m_VecUser.push_back(uid);
    }
}

uint32_t Chatroom::RemoveUser(uint32_t uid)
{
    return *(std::remove(m_VecUser.begin(), m_VecUser.end(), uid));
}

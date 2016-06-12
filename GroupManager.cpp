#include "GroupManager.h"
#include "iPublisher.h"

GroupManager::GroupManager(std::shared_ptr<iPublisher> spPublisher)
    : m_spPublisher(spPublisher)
{
}

GroupManager::~GroupManager()
{
}

int GroupManager::Init()
{
    m_spPublisher->Register(MSG_REQ_CHATROOM_LIST, this);
    m_spPublisher->Register(MSG_REQ_CREATE_CHATROOM, this);
    m_spPublisher->Register(MSG_REQ_JOIN_CHATROOM, this);
    m_spPublisher->Register(MSG_REQ_QUIT_CHATROOM, this);
    m_spPublisher->Register(MSG_REQ_CHATROOM_SPEAK, this);
    m_spPublisher->Register(MSG_REQ_ADMIN_SPEAK, this);
    return 0;
}

int GroupManager::Fini()
{
    m_spPublisher->Unregister(MSG_REQ_CHATROOM_LIST, this);
    m_spPublisher->Unregister(MSG_REQ_CREATE_CHATROOM, this);
    m_spPublisher->Unregister(MSG_REQ_JOIN_CHATROOM, this);
    m_spPublisher->Unregister(MSG_REQ_QUIT_CHATROOM, this);
    m_spPublisher->Unregister(MSG_REQ_CHATROOM_SPEAK, this);
    m_spPublisher->Unregister(MSG_REQ_ADMIN_SPEAK, this);
    return 0;
}

void GroupManager::HandleMessage(std::shared_ptr<RawMessage> spMsg)
{
    switch (spMsg->id()) {
        case MSG_REQ_ADMIN_SPEAK:
            HandleReqAdminSpeak(spMsg);
            break;
        case MSG_REQ_CHATROOM_SPEAK:
            HandleReqChatroomSpeak(spMsg);
            break;
        case MSG_REQ_QUIT_CHATROOM:
            HandleReqQuitChatroom(spMsg);
            break;
        case MSG_REQ_JOIN_CHATROOM:
            HandleReqJoinChatroom(spMsg);
            break;
        case MSG_REQ_CREATE_CHATROOM:
            HandleReqJoinChatroom(spMsg);
            break;
        case MSG_REQ_CHATROOM_LIST:
            HandleReqChatroomList(spMsg);
            break;
        default:
            assert(false);   
    }
}

void GroupManager::HandleReqChatroomList(std::shared_ptr<RawMessage> spMsg)
{
}

void GroupManager::HandleReqCreateChatroom(std::shared_ptr<RawMessage> spMsg)
{
}

void GroupManager::HandleReqJoinChatroom(std::shared_ptr<RawMessage> spMsg)
{
}

void GroupManager::HandleReqQuitChatroom(std::shared_ptr<RawMessage> spMsg)
{
}

void GroupManager::HandleReqChatroomSpeak(std::shared_ptr<RawMessage> spMsg)
{
}

void GroupManager::HandleReqAdminSpeak(std::shared_ptr<RawMessage> spMsg)
{
}

#include "ChatroomManager.h"
#include "iPublisher.h"
#include "CommonApi.h"
#include "User.h"
#include "NetApi.h"
#include "src/ChatRoom.pb.h"

ChatroomManager::ChatroomManager(std::shared_ptr<iPublisher> spPublisher)
: m_spPublisher(spPublisher)
{
}

ChatroomManager::~ChatroomManager()
{
}

int ChatroomManager::Init()
{
    m_spPublisher->Register(MSG_REQ_CHATROOM_LIST, this);
    m_spPublisher->Register(MSG_REQ_CREATE_CHATROOM, this);
    m_spPublisher->Register(MSG_REQ_JOIN_CHATROOM, this);
    m_spPublisher->Register(MSG_REQ_QUIT_CHATROOM, this);
    m_spPublisher->Register(MSG_REQ_CHATROOM_SPEAK, this);
    return 0;
}

int ChatroomManager::Fini()
{
    m_spPublisher->Unregister(MSG_REQ_CHATROOM_LIST, this);
    m_spPublisher->Unregister(MSG_REQ_CREATE_CHATROOM, this);
    m_spPublisher->Unregister(MSG_REQ_JOIN_CHATROOM, this);
    m_spPublisher->Unregister(MSG_REQ_QUIT_CHATROOM, this);
    m_spPublisher->Unregister(MSG_REQ_CHATROOM_SPEAK, this);
    return 0;
}

uint32_t ChatroomManager::GenerateChatroomId()
{
    static uint32_t i = 0;
    return ++i;
}

CHATROOM_DATA& ChatroomManager::CreateChatroom(
    const std::string& name,
    const std::string& content)
{
    uint32_t chatroom_id = GenerateChatroomId();

    CHATROOM_DATA data;
    data.set_id(chatroom_id);
    data.set_name(name);
    data.set_notice(content);

    std::pair<MapChatRoomTypeIterator, bool> result = m_mapChatroom.emplace(
        chatroom_id, data);

    return result.first->second;
}

void ChatroomManager::HandleMessage(std::shared_ptr<RawMessage> spMsg)
{
    switch (spMsg->id()) {
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

void ChatroomManager::HandleReqChatroomList(std::shared_ptr<RawMessage> spMsg)
{
    CHECK_NULL_ASSERT(spMsg);

    UserSmartPtr spUser = ::FindUserById(spMsg->uid());
    CHECK_NULL_ASSERT(spMsg);

    NotifyChatroomList notify;

    for (auto& item : m_mapChatroom) {
        CHATROOM_DATA* pData = notify.add_chatrooms();
        *pData = item.second;
    }

    NetApi::SendPacketToUser(spUser, notify);
}

void ChatroomManager::HandleReqCreateChatroom(std::shared_ptr<RawMessage> spMsg)
{
    CHECK_NULL_ASSERT(spMsg);
    ReqCreateChatroom req;
    req.ParseFromString(spMsg->strmsg());

    UserSmartPtr spUser = ::FindUserById(spMsg->uid());
    CHECK_NULL_ASSERT(spUser);

    CHATROOM_DATA data = CreateChatroom(req.name(), req.notice());

    NotifyCreateChatroomResult notify;
    notify.set_result(NCCR_SUCCESS);
    CHATROOM_DATA* pData = notify.mutable_chatroom();
    *pData = data;

    NetApi::SendPacketToUser(spUser, notify);

}

void ChatroomManager::HandleReqJoinChatroom(std::shared_ptr<RawMessage> spMsg)
{
    CHECK_NULL_ASSERT(spMsg);
    ReqJoinChatRoom req;
    req.ParseFromString(spMsg->strmsg());

    UserSmartPtr spUser = ::FindUserById(spMsg->uid());
    CHECK_NULL_ASSERT(spUser);
}

void ChatroomManager::HandleReqQuitChatroom(std::shared_ptr<RawMessage> spMsg)
{
}

void ChatroomManager::HandleReqChatroomSpeak(std::shared_ptr<RawMessage> spMsg)
{
}

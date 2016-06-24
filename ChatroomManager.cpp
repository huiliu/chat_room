#include "ChatroomManager.h"
#include "iPublisher.h"
#include "CommonApi.h"
#include "User.h"
#include "NetApi.h"
#include "Chatroom.h"

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

std::shared_ptr<Chatroom> ChatroomManager::CreateChatroom(
    const std::string& name,
    const std::string& content)
{
    uint32_t chatroom_id = GenerateChatroomId();

    CHATROOM_DATA data;
    data.set_id(chatroom_id);
    data.set_name(name);
    data.set_notice(content);

    std::shared_ptr<Chatroom> spChatroom = std::make_shared<Chatroom>(data);

    std::pair<MapChatRoomTypeIterator, bool> result = m_mapChatroom.emplace(
        chatroom_id, spChatroom);

    return result.first->second;
}

std::shared_ptr<Chatroom> ChatroomManager::FindChatroomById(uint32_t id)
{
    MapChatRoomTypeIterator it = m_mapChatroom.find(id);
    if (m_mapChatroom.end() != it) {
        return it->second;
    }
    return nullptr;
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
            HandleReqCreateChatroom(spMsg);
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
        // TODO: protobuf repeated field的赋值问题？
        pData->operator=(item.second->GetChatroomData());
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

    auto spChatroom = CreateChatroom(req.name(), req.notice());

    NotifyCreateChatroomResult notify;
    notify.set_result(NCCR_SUCCESS);
    CHATROOM_DATA& pData = *notify.mutable_chatroom();
    // TODO: protobuf 中自定义对象的赋值问题
    pData = spChatroom->GetChatroomData();

    NetApi::SendPacketToUser(spUser, notify);
}

void ChatroomManager::HandleReqJoinChatroom(std::shared_ptr<RawMessage> spMsg)
{
    CHECK_NULL_ASSERT(spMsg);
    ReqJoinChatRoom req;
    req.ParseFromString(spMsg->strmsg());

    UserSmartPtr spUser = ::FindUserById(spMsg->uid());
    CHECK_NULL_ASSERT(spUser);

    auto spChatroom = FindChatroomById(req.id());
    assert(spChatroom);
    if (spChatroom) {
        spUser->SetChatroomId(spChatroom->GetChatroomId());
        spChatroom->AddUser(spUser->GetUserId());
    }

    NotifyJoinChatroomResult notify;
    notify.set_result(NJCR_SUCCESS);
    CHATROOM_DATA* pData = notify.mutable_chatroom();
    // TODO: protobuf
    pData->operator=(spChatroom->GetChatroomData());

    NetApi::SendPacketToUser(spUser, notify);
}

void ChatroomManager::HandleReqQuitChatroom(std::shared_ptr<RawMessage> spMsg)
{
    CHECK_NULL_ASSERT(spMsg);
    ReqQuitChatroom req;
    req.ParseFromString(spMsg->strmsg());

    UserSmartPtr spUser = ::FindUserById(spMsg->uid());
    CHECK_NULL_ASSERT(spUser);

    auto spChatroom = FindChatroomById(spUser->GetChatroomId());
    if (spChatroom) {
        spChatroom->RemoveUser(spUser->GetUserId());
    }
    spUser->SetChatroomId(0);

    NotifyQuitChatroom notify;
    notify.set_result(NQC_SUCCESS);

    NetApi::SendPacketToUser(spUser, notify);
}

bool ChatroomManager::CanSpeakInChatroom(std::shared_ptr<User> spUser, const ReqChatroomSpeak& req, NOTIFY_SPEAK_FAILED_REASON& err)
{
    CHECK_NULL_RETURN(spUser, false);

    if (spUser->GetChatroomId() == 0) {
        err = NSFR_NOT_IN_CHATROOM;
        return false;
    }

    if (spUser->GetChatroomId() != req.id()) {
        err = NSFR_WRONG_CHATROOM;
        return false;
    }

    auto spChatroom = FindChatroomById(req.id());
    if (!spChatroom) {
        err = NSFR_NO_CHATROOM;
        return false;
    }
    return true;
}

void ChatroomManager::HandleReqChatroomSpeak(std::shared_ptr<RawMessage> spMsg)
{
    CHECK_NULL_ASSERT(spMsg);

    UserSmartPtr spUser = ::FindUserById(spMsg->uid());
    CHECK_NULL_ASSERT(spUser);

    ReqChatroomSpeak req;
    req.ParseFromString(spMsg->strmsg());

    NOTIFY_SPEAK_FAILED_REASON err;

    if (!CanSpeakInChatroom(spUser, req, err)) {
        NotifySpeakFailed notify;
        notify.set_result(err);

        NetApi::SendPacketToUser(spUser, notify);
        return;
    }
}

void ChatroomManager::ChatroomSpeak(User& speaker, uint32_t uChatroomId, const std::string& msg)
{
    NotifyChatroomSpeak notify;
    notify.set_speaker(speaker.GetUserId());
    notify.set_chatroom_id(uChatroomId);
    notify.set_content(msg);

    if (UPT_NORMAL == speaker.GetPriority()) {
        notify.set_type(CST_GENERAL);
    }
    else
    {
        notify.set_type(CST_ADMINISTRATOR);
    }

    auto spChatroom = FindChatroomById(uChatroomId);
    CHECK_NULL_ASSERT(spChatroom);

    auto Users = spChatroom->GetUsers();
    for (auto& uid : Users) {
        NetApi::SendPacketByUid(uid, notify);
    }
}

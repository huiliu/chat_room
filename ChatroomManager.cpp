#include "ChatroomManager.h"
#include "iPublisher.h"

ChatroomManager::ChatroomManager(std::shared_ptr<iPublisher> spPublisher)
: m_pPublisher(spPublisher)
{
}

void ChatroomManager::Init()
{
}

void ChatroomManager::Fini()
{
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
    std::pair<MapChatRoomTypeIterator, bool> result = m_mapChatroom.emplace(
        chatroom_id, chatroom_id, name, constent);

    if (result.second) {
        return result.first;
    }
}

void ChatroomManager::HandleMessage(std::shared_ptr<RawMessage> spMsg)
{
}



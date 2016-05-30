#ifndef __CHATROOMMANGER_H__
#define __CHATROOMMANGER_H__ 1

#include <map>
#include <string>
#include "iSubscriber.h"
#include "src/ChatRoom.pb.h"

typedef std::map<uint32_t, CHATROOM_DATA> MapChatRoomType;
typedef MapChatRoomType::iterator MapChatRoomTypeIterator;

class ChatroomManager : public iSubscriber
{
public:
    ChatroomManager(std::shared_ptr<iPublisher> spPublisher);
    virtual ~ChatroomManager ();

    void Init();
    void Fini();

    virtual void HandleMessage(std::shared_ptr<RawMessage> spMsg);

    CHATROOM_DATA&   CreateChatroom(const std::string& name,
                                    const std::string& content);

    const std::map<uint32_t, CHATROOM_DATA>&   GetAllChatromm() const;
    void ChatroomSpeak(uint32_t uid, uint32_t chatroom_id, const std::string& msg);

    void OnEnterChatroom(uint32_t uid, uint32_t chatroom_id);
    void OnExitChatroom(uint32_t uid, uint32_t chatroom_id);

private:

    uint32_t GenerateChatroomId();

    void    HandleReqCreateChatroom(std::shared_ptr<RawMessage> spMsg);
    void    HandleReqChatroomList(std::shared_ptr<RawMessage> spMsg);
    void    HandleReqJoinChatroom(std::shared_ptr<RawMessage> spMsg);
    void    HandleReqQuitChatroom(std::shared_ptr<RawMessage> spMsg);
    void    HandleReqSpeakInChatroom(std::shared_ptr<RawMessage> spMsg);

private:
    std::shared_ptr<iPublisher> m_pPublisher;
    MapChatRoomType             m_mapChatroom;
};

#endif /* ifndef __CHATROOMMANGER_H__ */

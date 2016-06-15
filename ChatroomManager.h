#ifndef __CHATROOMMANGER_H__
#define __CHATROOMMANGER_H__ 1

#include <map>
#include <string>
#include "iSubscriber.h"
#include "src/ChatRoom.pb.h"

class Chatroom;
class User;

typedef std::map<uint32_t, std::shared_ptr<Chatroom>> MapChatRoomType;
typedef MapChatRoomType::iterator MapChatRoomTypeIterator;

class ChatroomManager : public iSubscriber
{
public:
    ChatroomManager(std::shared_ptr<iPublisher> spPublisher);
    ~ChatroomManager ();

    ChatroomManager(const ChatroomManager& other) = delete;
    ChatroomManager(ChatroomManager&& other)      = delete;
    ChatroomManager& operator=(const ChatroomManager& other) = delete;

    int Init();
    int Fini();

    virtual void HandleMessage(std::shared_ptr<RawMessage> spMsg);

    std::shared_ptr<Chatroom>      CreateChatroom(const std::string& name,
                                                const std::string& content);
    std::shared_ptr<Chatroom>      FindChatroomById(uint32_t id);

    void ChatroomSpeak(User& speaker, uint32_t uChatroomId, const std::string& msg);

    void OnEnterChatroom(uint32_t uid, uint32_t uChatroomId);
    void OnExitChatroom(uint32_t uid, uint32_t uChatroomId);

private:

    uint32_t GenerateChatroomId();

    // 请求聊天室列表
    void    HandleReqChatroomList(std::shared_ptr<RawMessage> spMsg);
    // 请求创建一个聊天室
    void    HandleReqCreateChatroom(std::shared_ptr<RawMessage> spMsg);
    // 请求加入聊天室
    void    HandleReqJoinChatroom(std::shared_ptr<RawMessage> spMsg);
    // 请求退出聊天室
    void    HandleReqQuitChatroom(std::shared_ptr<RawMessage> spMsg);
    // 请求发言聊天室
    void    HandleReqChatroomSpeak(std::shared_ptr<RawMessage> spMsg);

    bool    CanSpeakInChatroom(std::shared_ptr<User> spUser, const ReqChatroomSpeak& req, NOTIFY_SPEAK_FAILED_REASON& result);

private:
    std::shared_ptr<iPublisher> m_spPublisher;
    MapChatRoomType             m_mapChatroom;
};

#endif /* ifndef __CHATROOMMANGER_H__ */

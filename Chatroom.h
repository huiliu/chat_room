#ifndef __CHATROOM_CHATROOM_H__
#define __CHATROOM_CHATROOM_H__ 1
// 
// 用于管理chatroom
//

#include "src/ChatRoom.pb.h"
#include <vector>

class Chatroom
{
public:
    explicit Chatroom(const CHATROOM_DATA& data);
    ~Chatroom ();

    void                SetChatroomId(uint32_t cid);
    uint32_t            GetChatroomId() const;

    void                SetChatroomName(const std::string& name);
    const std::string&  GetChatroomName() const;

    void                SetChatroomNotice(const std::string& notice);
    const std::string&  GetChatroomNotice() const;

    const CHATROOM_DATA& GetChatroomData() const { return m_BasicData; }

    bool                IsInChatroom(uint32_t uid);
    void                AddUser(uint32_t uid);
    uint32_t            RemoveUser(uint32_t uid);
    const std::vector<uint32_t>&   GetUsers() const { return m_VecUser; };

private:
    CHATROOM_DATA           m_BasicData;
    std::vector<uint32_t>   m_VecUser;
};
#endif /* ifndef __CHATROOM_CHATROOM_H__ */

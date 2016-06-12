#ifndef __CHATROOM_GROUPMANAGER_H__
#define __CHATROOM_GROUPMANAGER_H__ 1
//
// file: GroupManager.h
// date:
// description:
//      聊天室操作
//
#include "iSubscriber.h"

class GroupManager : public iSubscriber
{
public:
    GroupManager(std::shared_ptr<iPublisher> spPublisher);
    ~GroupManager ();

    int Init();
    int Fini();

    virtual void HandleMessage(std::shared_ptr<RawMessage> spMsg);

private:

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
    // 管理员发言
    void    HandleReqAdminSpeak(std::shared_ptr<RawMessage> spMsg);

private:
    std::shared_ptr<iPublisher>     m_spPublisher;
    /* data */
};
#endif /* ifndef __CHATROOM_GROUPMANAGER_H__ */

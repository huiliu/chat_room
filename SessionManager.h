#ifndef __CHATROOM_SESSIONMANAGER_H__
#define __CHATROOM_SESSIONMANAGER_H__ 1
//
// file: SessionManager.h
// Date:
// Description:
//      用于管理帐号登陆验证，下线等
//
//
#include "iSubscriber.h"

class SessionManager : public iSubscriber
{
public:
    SessionManager(std::shared_ptr<iPublisher> spMsg);
    ~SessionManager ();

    int Init();
    int Fini();

    SessionManager(const SessionManager&) = delete;
    SessionManager(SessionManager&&)      = delete;
    SessionManager& operator=(const SessionManager&) = delete;

    virtual void HandleMessage(std::shared_ptr<RawMessage> spMsg);

private:
    void        HandleVersionCheck(std::shared_ptr<RawMessage> spMsg);
    void        HandleReqLogin(std::shared_ptr<RawMessage> spMsg);
    void        HandleLogout(std::shared_ptr<RawMessage> spMsg);

    int         OnLogin(uint32_t uid);
    int         OnLogout(uint32_t uid);
private:
    std::shared_ptr<iPublisher>     m_spPublisher;
};

#endif /* ifndef __CHATROOM_SESSIONMANAGER_H__ */

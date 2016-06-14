#ifndef __CHATROOM_USER_H__
#define __CHATROOM_USER_H__ 1
#include "CommonDataType.h"
#include "src/UserData.pb.h"

class User
{
public:
    explicit User(const UserData&);
    ~User();


    void                SetUserName(const std::string& name);
    const std::string&  GetUserName() const { return m_UserData.name(); }

    void                SetUserId(uint32_t id);
    uint32_t            GetUserId() const { return m_UserData.id(); }

    void                SetConnId(ConnID connId) { m_ConnId = connId; }
    ConnID              GetConnId() const { return m_ConnId; }

private:
    UserData    m_UserData;
    ConnID      m_ConnId;
};

typedef std::shared_ptr<User> UserSmartPtr;
#endif /* ifndef __CHATROOM_USER_H__ */

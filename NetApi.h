#ifndef __CHATROOM_NETAPI_H__
#define __CHATROOM_NETAPI_H__ 1
#include "User.h"
#include "ServerBase.h"
#include "ConnectionManager.h"
#include "CommonApi.h"

namespace NetApi
{

template<typename T>
void SendPacketToUser(const std::shared_ptr<User> spUser, T& msg)
{
    ServerBase* pBase = ServerBase::GetInstance();
    std::shared_ptr<ConnectionManager> spConnMgr = pBase->GetConnMgr();
    CHECK_NULL_ASSERT(spConnMgr);

    std::string strMsg;
    msg.SerializeToString(&strMsg);

    std::shared_ptr<RawMessage> spMsg = std::make_shared<RawMessage>();
    spMsg->set_id(msg.msg_id());
    spMsg->set_uid(spUser->GetUserId());
    spMsg->set_timestamp(0);
    spMsg->set_strmsg(strMsg);

    spConnMgr->PutInSendQueue(spUser->GetConnId(), spMsg);
}

template<typename T>
void SendPacket(ConnID connId, T& msg)
{
    ServerBase* pBase = ServerBase::GetInstance();
    std::shared_ptr<ConnectionManager> spConnMgr = pBase->GetConnMgr();
    CHECK_NULL_ASSERT(spConnMgr);

    std::string strMsg;
    msg.SerializeToString(&strMsg);

    std::shared_ptr<RawMessage> spMsg = std::make_shared<RawMessage>();
    spMsg->set_id(msg.msg_id());
    spMsg->set_timestamp(0);
    spMsg->set_strmsg(strMsg);

    spConnMgr->PutInSendQueue(connId, spMsg);
}



}
#endif /* ifndef __CHATROOM_NETAPI_H__ */

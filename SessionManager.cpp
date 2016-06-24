#include "SessionManager.h"
#include "iPublisher.h"
#include "src/Login.pb.h"
#include "src/version.pb.h"
#include "NetApi.h"
#include "User.h"
#include "UserManager.h"
#include "CommonApi.h"

SessionManager::SessionManager(std::shared_ptr<iPublisher> spPublisher)
    : m_spPublisher(spPublisher)
{
}

SessionManager::~SessionManager()
{
}

int SessionManager::Init()
{
    m_spPublisher->Register(MSG_CHECK_VERSION, this);
    m_spPublisher->Register(MSG_REQ_LOGIN, this);
    return 0;
}

int SessionManager::Fini()
{
    m_spPublisher->Unregister(MSG_REQ_LOGIN, this);
    m_spPublisher->Unregister(MSG_CHECK_VERSION, this);
    return 0;
}

void SessionManager::HandleMessage(std::shared_ptr<RawMessage> spMsg)
{
    switch (spMsg->id()) {
        case MSG_CHECK_VERSION:
            HandleVersionCheck(spMsg);
            break;
        case MSG_REQ_LOGIN:
            HandleReqLogin(spMsg);
            break;
        default:
            assert(false);
    }
}

void SessionManager::HandleVersionCheck(std::shared_ptr<RawMessage> spMsg)
{
    CHECK_NULL_ASSERT(spMsg);
    DEBUG_LOG("收到了版本验证回复！");

    NotifyVersionCheck notify;
    notify.ParseFromString(spMsg->strmsg());
    DEBUG_LOG(notify.result());
}

void SessionManager::HandleReqLogin(std::shared_ptr<RawMessage> spMsg)
{
    CHECK_NULL_ASSERT(spMsg);
    ReqLogin req;
    req.ParseFromString(spMsg->strmsg());
    UserData data;

    data.set_id(spMsg->clientid());   // 使用连接id
    data.set_name(req.username());

    DEBUG_LOG("收到了登陆请求！");
    DEBUG_LOG(req.username());

    auto spUser = ServerBase::GetInstance()->GetUserMgr()->CreateUser(data);
    spUser->SetConnId(spMsg->clientid());

    NotifyLoginResult notify;
    notify.set_result(NLR_SUCCESS);

    NetApi::SendPacketToUser(spUser, notify);
}

void SessionManager::HandleLogout(std::shared_ptr<RawMessage> spMsg)
{
}

int SessionManager::OnLogin(uint32_t uid)
{
    return 0;
}

int SessionManager::OnLogout(uint32_t uid)
{
    return 0;
}

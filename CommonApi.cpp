#include "CommonApi.h"
#include "User.h"
#include "ServerBase.h"
#include "UserManager.h"

UserSmartPtr FindUserById(uint32_t uid)
{
    ServerBase* pBase = ServerBase::GetInstance();
    std::shared_ptr<UserManager> spUserMgr = pBase->GetUserMgr();
    CHECK_NULL_RETURN(spUserMgr, nullptr);

    return spUserMgr->FindUser(uid);
}

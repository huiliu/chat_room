#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__ 1
#include "iSubscriber.h"
#include "src/UserData.pb.h"
#include <map>

class UserManager : public iSubscriber
{
public:
    UserManager(std::shared_ptr<iPublisher> spPublisher);
    ~UserManager ();

    int Init();
    int Fini();

    virtual void HandleMessage(std::shared_ptr<RawMessage> spMsg);

    UserData&   FindUser(uint32_t uid);

private:
    std::shared_ptr<iPublisher> m_spPublisher;
    std::map<uint32_t, UserData> m_mapUserData;
};
#endif /* ifndef __USERMANAGER_H__ */

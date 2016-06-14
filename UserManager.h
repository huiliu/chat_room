#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__ 1
#include "iSubscriber.h"
#include "src/UserData.pb.h"
#include <map>

class User;

typedef std::map<uint32_t, std::shared_ptr<User>> UserMapType;
typedef UserMapType::iterator   UserMapTypeIterator;

class UserManager : public iSubscriber
{
public:
    UserManager(std::shared_ptr<iPublisher> spPublisher);
    ~UserManager ();

    int Init();
    int Fini();

    virtual void HandleMessage(std::shared_ptr<RawMessage> spMsg);

    std::shared_ptr<User>   CreateUser(const UserData& data);
    std::shared_ptr<User>   FindUser(uint32_t uid);

private:
    std::shared_ptr<iPublisher> m_spPublisher;
    UserMapType m_mapUserData;
};
#endif /* ifndef __USERMANAGER_H__ */

#ifndef __CHATROOM_COMMONAPI_H_
#define __CHATROOM_COMMONAPI_H_ 1
#include <cassert>
#include <memory>

class User;

#define CHECK_NULL_ASSERT(p) \
    if (!p) {\
        assert(false);\
        return;\
    }

#define CHECK_NULL_RETURN(p, ret) \
    if (!p) {\
        assert(false);\
        return (ret);\
    }

std::shared_ptr<User> FindUserById(uint32_t uid);

#endif /* ifndef __CHATROOM_COMMONAPI_H_ */

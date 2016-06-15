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

#ifdef DEBUG
    #include <iostream>
    #define DEBUG_LOG(msg) \
        std::cout << msg << std::endl
#else
    #define DEBUG_LOG(msg) do{}while(0)
#endif


std::shared_ptr<User> FindUserById(uint32_t uid);

#endif /* ifndef __CHATROOM_COMMONAPI_H_ */

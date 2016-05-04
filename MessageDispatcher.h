#ifndef __MESSAGEMANAGER_H__
#define __MESSAGEMANAGER_H__

#include "iPublisher.h"


// 消息管理器，用于分发网络消息，时间脉冲
class MessageDispatcher : public iPublisher
{
public:
    MessageDispatcher();
    virtual ~MessageDispatcher();

private:
    /* data */
};
#endif /* ifndef __MESSAGEMANAGER_H__ */

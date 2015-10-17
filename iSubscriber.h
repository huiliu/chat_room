#ifndef __ISUBSCRIBER_H__
#define __ISUBSCRIBER_H__
#include <memory>

class iPublisher;
class RawMessage;

class iSubscriber
{
public:
    iSubscriber();
    virtual ~iSubscriber() = 0;

    virtual void HandleMessage(std::shared_ptr<RawMessage> msg) = 0;
};


#endif

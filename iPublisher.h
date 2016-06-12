#ifndef __IPUBLISHER_H__
#define __IPUBLISHER_H__

#include "src/MessageID.pb.h"
#include <map>
#include <vector>
#include <memory>

class iSubscriber;

class iPublisher
{
public:
    virtual ~iPublisher() = 0;

     //将订阅者加入到消息队列
    virtual bool Register(MessageID id, iSubscriber* psub);

    // 将订阅者从消息队列移除
    virtual bool Unregister(MessageID id, iSubscriber* psub);

    // 向订阅者广播消息
    virtual void Notify(std::shared_ptr<RawMessage> pMsg);

protected:
    iPublisher();

private:
    typedef std::vector<iSubscriber*> VEC_SUBSCRIBER;
    typedef std::map<uint32_t, VEC_SUBSCRIBER> MAP_MSG_SUBSCRIBER;
    MAP_MSG_SUBSCRIBER m_subscriber;
};

#endif

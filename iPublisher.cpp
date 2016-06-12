#include "iPublisher.h"
#include "iSubscriber.h"

iPublisher::iPublisher()
: m_subscriber()
{
}

iPublisher::~iPublisher()
{
    for (auto subs : m_subscriber) {
        VEC_SUBSCRIBER& vecSub = subs.second;        
        for (auto isub : vecSub) {
            delete isub;
            isub = nullptr;
        }
        vecSub.clear();
    }
    m_subscriber.clear();
}

bool iPublisher::Register(MessageID id, iSubscriber* psub)
{
    m_subscriber[id].push_back(psub);
    return true;
}

bool iPublisher::Unregister(MessageID id, iSubscriber* psub)
{
    MAP_MSG_SUBSCRIBER::iterator it = m_subscriber.find(id);
    if (m_subscriber.end() == it)
    {
        // 警告信息，此消息没有订阅者
        return true;
    }

    VEC_SUBSCRIBER& vecSub = it->second;
    VEC_SUBSCRIBER::iterator itSub = vecSub.begin();
    for (; vecSub.end() !=itSub; ++itSub) {
        // 比较两个指针，感觉怪怪的
        if (*itSub == psub) {
            vecSub.erase(itSub);
            return true;
        }
    }
    return true;
}

void iPublisher::Notify(std::shared_ptr<RawMessage> pMsg)
{
    MessageID MsgId = pMsg->id();

    MAP_MSG_SUBSCRIBER::iterator it = m_subscriber.find(MsgId);
    if (m_subscriber.end() == it) {
        return;
    }

    for (auto& subscriber : it->second) {
        subscriber->HandleMessage(pMsg);
    }
}

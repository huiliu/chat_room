#ifndef __NETSUBSCRIBER_H__
#define __NETSUBSCRIBER_H__
#include <memory>
#include "iSubscriber.h"

class iPublisher;

class NetSubscriber : public iSubscriber
{
public:
    NetSubscriber(std::shared_ptr<iPublisher> pPub);
    ~NetSubscriber();

    void Init();
    void Fini();

    virtual void HandleMessage(std::shared_ptr<RawMessage> pMsg);

private:
    // 处理登陆请求
    void HandleReqLogin(std::shared_ptr<RawMessage> pMsg);
    // 处理系统时间脉冲
    void HandleSystemTick(std::shared_ptr<RawMessage> pMsg);

private:
    std::shared_ptr<iPublisher> m_pPublisher;
};


#endif

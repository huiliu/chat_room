#ifndef __NETSUBSCRIBER_H__
#define __NETSUBSCRIBER_H__
#include <memory>
#include "iSubscriber.h"

class iPublisher;

class NetSubscriber : public iSubscriber
{
public:
    NetSubscriber(iPublisher* pPub);
    ~NetSubscriber();

    void Init();
    void Fini();

    virtual void HandleMessage(std::shared_ptr<RawMessage> pMsg);

private:
    void HandleReqLogin(std::shared_ptr<RawMessage> pMsg);

private:
    iPublisher* m_pPublisher;
};


#endif

#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__
#include <queue>

#include "iSubscriber.h"

class iEvent;

class EventManager : public iSubscriber
{
public:
    EventManager(iPublisher* pPub);
    virtual ~EventManager();

    int Init();
    int Fini();

    virtual void HandleMessage(std::shared_ptr<RawMessage> spMsg);
    void    ScheduleEvent(iEvent*, time_t relative);

private:
    void    HandleTimeTick(std::shared_ptr<RawMessage> spMsg);

private:
    struct QueueEntry
    {
        QueueEntry(time_t t, iEvent* e) : m_time(t), m_event(e) {}
        time_t  m_time;
        iEvent* m_event;
    };
    iPublisher*     m_pPublisher;
    typedef std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry> > EventQueue;
    EventQueue m_eventQueue;
};

#endif

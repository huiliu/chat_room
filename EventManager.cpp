#include <cassert>
#include <iostream>
#include "EventManager.h"
#include "iPublisher.h"
#include "src/MessageID.pb.h"


EventManager::EventManager(iPublisher* pPub)
: m_pPublisher(pPub)
{
}

EventManager::~EventManager()
{
    Fini();
}

int EventManager::Init()
{
    if (nullptr == m_pPublisher) {
        assert(false);
        return -1;
    }
    m_pPublisher->Attach(MSG_TIME_TICK, this);
    return 0;
}

int EventManager::Fini()
{
    m_pPublisher->Detach(MSG_TIME_TICK, this);

    return 0;
}

void EventManager::HandleMessage(std::shared_ptr<RawMessage> spMsg)
{
    if (nullptr == spMsg) {
        
        assert(false);
        return;
    }

    switch (spMsg->id()) {
        case MSG_TIME_TICK:
            {
                HandleTimeTick(spMsg);
            }
            break;
        default:
            assert(false);
            break;
    }
}

void EventManager::HandleTimeTick(std::shared_ptr<RawMessage> spMsg)
{
    std::cout << "Recevied Time Tick!" << std::endl;    
}

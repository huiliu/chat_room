#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "src/MessageID.pb.h"
#include "Timer.h"
#include "iPublisher.h"

const int TIME_INTEVAL = 1;

Timer::Timer(boost::asio::io_service& io, std::shared_ptr<iPublisher> pPublisher)
: m_timer(io, boost::posix_time::seconds(TIME_INTEVAL))
, m_pPublisher(pPublisher)
, m_spMsg(nullptr)
{
}

Timer::~Timer()
{
    Fini();
}

int Timer::Init()
{
    m_spMsg = std::make_shared<RawMessage>(RawMessage());
    return 0;
}

int Timer::Fini()
{
    return 0;
}

void Timer::Start()
{
    m_timer.async_wait(boost::bind(&Timer::OnTick, this));
}

void Timer::OnTick()
{
    // 时间脉冲消息
    m_pPublisher->Notify(m_spMsg);

    m_timer.expires_at(m_timer.expires_at() + boost::posix_time::seconds(TIME_INTEVAL));
    m_timer.async_wait(boost::bind(&Timer::OnTick, this));
}

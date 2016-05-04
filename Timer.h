#ifndef __TIMER_H__
#define __TIMER_H__
#include <boost/asio.hpp>
#include <memory>

class iPublisher;
class RawMessage;

/*
 * 时间消息发生器
 */
class Timer
{
public:
    Timer(boost::asio::io_service& io_service, std::shared_ptr<iPublisher> pPublisher);
    virtual ~Timer();

    int Init();
    int Fini();
    // 启动定时器
    void Start();

private:

    void OnTick();

private:
    boost::asio::deadline_timer m_timer;
    std::shared_ptr<iPublisher> m_pPublisher;
    std::shared_ptr<RawMessage> m_spMsg;
};

#endif

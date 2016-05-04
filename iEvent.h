#ifndef __IEVENT_H__
#define __IEVENT_H__

class iEvent
{
public:
    iEvent ();
    virtual ~iEvent ();

    // 判断当前事件是否可以被触发
    virtual bool CanbeTrige();
    // 触发事件,由EventManager调用
    virtual void Triger();

    // 取消事件。使得事件不可被触发
    void Cancel();

private:
    bool m_canceled;
};

#endif

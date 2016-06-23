#ifndef __CHATROOM_ANALYSIS_H__
#define __CHATROOM_ANALYSIS_H__ 1
// 
// 一些分析接口
//
//
#include <cstdint>

template<typename T>
class iCountSum
{
public:
    iCountSum();
    virtual ~iCountSum () = 0;

    uint64_t    GetCountSum() const;

private:
    static uint64_t    m_Count;
};

template<typename T>
uint64_t iCountSum<T>::m_Count = 0;

template<typename T>
iCountSum<T>::iCountSum()
{
    ++m_Count;
}

template<typename T>
iCountSum<T>::~iCountSum()
{
    --m_Count;
}

template<typename T>
uint64_t iCountSum<T>::GetCountSum() const
{
    return m_Count;
}
#endif /* ifndef __CHATROOM_ANALYSIS_H__ */

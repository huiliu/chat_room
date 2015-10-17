#ifndef __NETPUBLISHER_H__
#define __NETPUBLISHER_H__
#include "iPublisher.h"

class NetPublisher : public iPublisher
{
protected:
    NetPublisher& operator=(const NetPublisher& );
    NetPublisher(const NetPublisher&);
public:
    NetPublisher();
    ~NetPublisher();

};

#endif

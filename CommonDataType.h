#ifndef __COMMONDATA_TYPE_H__
#define __COMMONDATA_TYPE_H__
#include <boost/asio.hpp>
#include <boost/assert.hpp>
#include <memory>
#include "src/MessageID.pb.h"

typedef std::shared_ptr<RawMessage>         SpRawMessage;
typedef uint32_t                            ConnID;
typedef std::pair<ConnID, SpRawMessage>     NetMessageEntity;

#endif /* ifndef __COMMONDATA_TYPE_H__ */

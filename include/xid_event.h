#ifndef __XID_EVENT_H__
#define __XID_EVENT_H__

#include "log_event.h"

namespace mysql_replicator {
class XidEvent : public LogEvent {
public:
    XidEvent(std::shared_ptr<BinlogEventHeader> header) : LogEvent(header) {}
    virtual void fromStream(std::istream&);
    virtual void printPacket();
private:
    uint8_t xid_;
};
}
#endif //__XID_EVENT_H__

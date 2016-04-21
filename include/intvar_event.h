#ifndef __INTVAR_EVENT_H__
#define __INTVAR_EVENT_H__

#include "log_event.h"

namespace mysql_replicator {
class IntvarEvent : public LogEvent {
public:
    IntvarEvent(std::shared_ptr<BinlogEventHeader> header) : LogEvent(header) {}
    virtual void fromStream(std::istream&);
    virtual void printPacket();
private:
    uint8_t type_;
    uint64_t value_;
};
}
#endif //__INTVAR_EVENT_H__

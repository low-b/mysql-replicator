#ifndef __ROTATE_EVENT_H__
#define __ROTATE_EVENT_H__

#include "log_event.h"

namespace mysql_replicator {
class RotateEvent : public LogEvent {
public:
    RotateEvent(std::shared_ptr<BinlogEventHeader> header) :
        LogEvent(header) {}
    virtual void fromStream(std::istream&);
    virtual void printPacket();
    uint64_t get_position() {
        return position_;
    }
    std::string get_next_binlog_name() {
        return next_binlog_name_;
    }
private:
    uint64_t position_;
    std::string next_binlog_name_;
};
}
#endif //__ROTATE_EVENT_H__

#ifndef __LOG_EVENT_H__
#define __LOG_EVENT_H__

#include <memory>
#include "binlog_event_header.h"

namespace mysql_replicator {
class LogEvent : public Packet {
public:
    LogEvent(std::shared_ptr<BinlogEventHeader> header)
        : header_(header) {}
    std::shared_ptr<BinlogEventHeader> get_header() {
        return header_;
    }
private:
    std::shared_ptr<BinlogEventHeader> header_;

};
}
#endif //__LOG_EVENT_H__

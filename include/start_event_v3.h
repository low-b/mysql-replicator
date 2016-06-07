#ifndef __START_EVENT_V3_H__
#define __START_EVENT_V3_H__

#include "log_event.h"

namespace mysql_replicator {
class StartEventV3 : public LogEvent {
public:
    StartEventV3(std::shared_ptr<BinlogEventHeader> header) :
        LogEvent(header) {}
    virtual void fromStream(std::istream&);
    virtual void printPacket();
private:
    uint16_t binlog_version_;
    std::string mysql_server_version_;
    uint32_t create_timestamp_;
};
}
#endif //__LOG_EVENT_V3_H__

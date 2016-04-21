#ifndef __QUERY_EVENT_H__
#define __QUERY_EVENT_H__

#include "log_event.h"

namespace mysql_replicator {
class QueryEvent : public LogEvent {
public:
    QueryEvent(std::shared_ptr<BinlogEventHeader> header) : LogEvent(header) {}
    virtual void fromStream(std::istream&);
    virtual void printPacket();
private:
    uint32_t slave_proxy_id_;
    uint32_t exec_time_;
    uint8_t schema_len_;
    uint16_t error_code_;
    uint16_t status_vars_len_;
    std::string status_vars_;
    std::string schema_;
    uint8_t fill_;
    std::string query_;
};
}
#endif //__QUERY_EVENT_H__

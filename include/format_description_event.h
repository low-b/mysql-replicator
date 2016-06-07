#ifndef __FORMAT_DESCRIPTION_EVENT_H__
#define __FORMAT_DESCRIPTION_EVENT_H__

#include <vector>
#include "log_event.h"

namespace mysql_replicator {
class FormatDescriptionEvent : public LogEvent {
public:
    FormatDescriptionEvent(uint8_t binlog_version,
            const std::string& mysql_server_version);
    FormatDescriptionEvent(std::shared_ptr<BinlogEventHeader> header) :
        LogEvent(header) {}
    virtual void fromStream(std::istream&);
    virtual void printPacket();
    uint8_t get_post_header_len(uint8_t event_type) {
        return post_header_lens_[event_type];
    }
private:
    uint16_t binlog_version_;
    std::string mysql_server_version_;
    uint32_t create_timestamp_;
    uint8_t event_header_len_;
    std::vector<uint8_t> post_header_lens_;
    uint8_t event_type_num_;
};
}
#endif //__FORMAT_DESCRIPTION_EVENT_H__

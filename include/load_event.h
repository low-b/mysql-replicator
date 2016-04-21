#ifndef __LOAD_EVENT_H__
#define __LOAD_EVENT_H__

#include <vector>
#include "log_event.h"

namespace mysql_replicator {
class LoadEvent : public LogEvent {
public:
    LoadEvent(std::shared_ptr<BinlogEventHeader> header) : LogEvent(header) {}
    virtual void fromStream(std::istream&);
    virtual void printPacket();
private:
    uint32_t slave_proxy_id_;
    uint32_t exec_time_;
    uint32_t skip_lines_;
    uint8_t table_name_len_;
    uint8_t schema_name_len_;
    uint32_t num_fields_;
    uint8_t field_term_;
    uint8_t enclosed_by_;
    uint8_t line_term_;
    uint8_t line_start_;
    uint8_t escaped_by_;
    uint8_t opt_flags_;
    uint8_t empty_flags_;
    std::vector<uint8_t> field_name_lens_;
    std::vector<std::string> field_names_;
    std::string table_name_;
    std::string schema_name_;
    std::string file_name_;
};
}
#endif //__LOAD_EVENT_H__

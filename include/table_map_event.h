#ifndef __TABLE_MAP_EVENT_H__
#define __TABLE_MAP_EVENT_H__

#include <vector>
#include <boost/dynamic_bitset.hpp>
#include "log_event.h"

namespace mysql_replicator {
class TableMapEvent : public LogEvent {
public:
    TableMapEvent(std::shared_ptr<BinlogEventHeader> header) :
        LogEvent(header) {}
    virtual void fromStream(std::istream&);
    virtual void printPacket();
    uint64_t get_table_id() {
        return table_id_;
    }
    std::string get_schema_name() {
        return schema_name_;
    }
    std::string get_table_name() {
        return table_name_;
    }
    const std::vector<uint8_t> & get_column_type_def() {
        return column_type_def_;
    }
    const std::vector<uint16_t> & get_column_meta_def() {
        return column_meta_def_;
    }
private:
    uint64_t table_id_;
    uint16_t flags_;
    uint8_t schema_name_len_;
    std::string schema_name_;
    uint8_t fill1_;
    uint8_t table_name_len_;
    std::string table_name_;
    uint8_t fill2_;
    uint64_t column_count_;
    std::vector<uint8_t> column_type_def_;
    std::vector<uint16_t> column_meta_def_;
    boost::dynamic_bitset<> null_bitset_;
};
}
#endif //__TABLE_MAP_EVENT_H__

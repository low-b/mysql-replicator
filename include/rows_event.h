#ifndef __ROWS_EVENT_H__
#define __ROWS_EVENT_H__

#include <vector>
#include <unordered_map>
#include <boost/dynamic_bitset.hpp>
#include "db_meta.h"
#include "format_description_event.h"
#include "log_event.h"
#include "table_map_event.h"
#include "binlog_event.pb.h"

namespace mysql_replicator {
class RowsEvent : public LogEvent {
public:
    RowsEvent(std::shared_ptr<BinlogEventHeader> header,
            uint8_t post_header_len) :
        LogEvent(header),
        post_header_len_(post_header_len)
    {}
    void set_db_meta(std::shared_ptr<DbMeta> db_meta) {
        db_meta_ = db_meta;
    }
    void set_table_map_event(const std::unordered_map<uint64_t,
            std::shared_ptr<TableMapEvent> >& table_map) {
        if (table_map.find(table_id_) != table_map.end()) {
            table_map_event_ = table_map.at(table_id_);
        }
    }
    virtual void fromStream(std::istream &is);
    virtual void printPacket();
    virtual void buildEventProto(std::vector<std::shared_ptr<BinlogEvent> >& event_vec);
    virtual void buildRowProto(std::istream &is,
            Row* row,
            std::shared_ptr<TableMeta> table_meta,
            const std::vector<uint8_t>& column_type_def,
            const std::vector<uint16_t>& column_meta_def,
            size_t& current_size,
            bool is_new);
private:
    uint8_t post_header_len_;
    uint64_t table_id_;
    uint16_t flags_;
    uint16_t extra_data_len_;
    std::string extra_data_;
    uint64_t column_count_;
    boost::dynamic_bitset<> columns_before_set_;
    boost::dynamic_bitset<> columns_after_set_;
    std::string row_data_;

    std::shared_ptr<DbMeta> db_meta_;
    std::shared_ptr<TableMapEvent> table_map_event_;
};
}
#endif //__ROWS_EVENT_H__

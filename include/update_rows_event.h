#ifndef __ROWS_EVENT_H__
#define __ROWS_EVENT_H__

#include <vector>
#include <boost/dynamic_bitset.hpp>
#include "log_event.h"

namespace mysql_replicator {
class RowsEvent : public LogEvent {
public:
    RowsEvent(std::shared_ptr<BinlogEventHeader> header, uint8_t version) : LogEvent(header), version_(version) {}
    virtual void fromStream(std::istream&);
    virtual void printPacket();
private:
    uint8_t version_;
    uint64_t table_id_;
    uint16_t flags_;
    uint16_t extra_data_len_;
    std::string extra_data_;
    uint64_t column_nums_;
    boost::dynamic_bitset<> columns_;
    boost::dynamic_bitset<> change_columns_;
    std::string row_data_;
};
}
#endif //__ROWS_EVENT_H__

#ifndef __BINLOG_EVENT_FACTORY_H__
#define __BINLOG_EVENT_FACTORY_H__

#include <boost/noncopyable.hpp>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>
#include "format_description_event.h"
#include "binlog_event_header.h"
#include "db_meta.h"
#include "table_map_event.h"

namespace mysql_replicator {
class BinlogEventFactory : public boost::noncopyable  {
public:
    BinlogEventFactory() = default;
    std::shared_ptr<LogEvent> create(boost::asio::streambuf& packet_buf);
    void set_db_meta(std::shared_ptr<DbMeta> db_meta) {
        db_meta_ = db_meta;
    }
    void set_format_description_event(std::shared_ptr<FormatDescriptionEvent> event) {
        format_description_event_ = event;
    }
private:
    std::shared_ptr<FormatDescriptionEvent> format_description_event_;
    std::shared_ptr<DbMeta> db_meta_;
    std::unordered_map<uint64_t, std::shared_ptr<TableMapEvent> > table_map_;
};
}
#endif // __BINLOG_EVENT_FACTORY_H__


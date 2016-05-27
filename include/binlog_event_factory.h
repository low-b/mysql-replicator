#ifndef __BINLOG_EVENT_FACTORY_H__
#define __BINLOG_EVENT_FACTORY_H__

#include <boost/noncopyable.hpp>
#include <memory>
#include "format_description_event.h"
#include "binlog_event_header.h"

namespace mysql_replicator {
class BinlogEventFactory : public boost::noncopyable  {
public:
    static BinlogEventFactory* instance() {
        static BinlogEventFactory factory;
        return &factory;
    }
    std::shared_ptr<LogEvent> create(std::shared_ptr<BinlogEventHeader> event_header);
private:
    BinlogEventFactory() = default;
    std::shared_ptr<FormatDescriptionEvent> _format_event;
};
}
#endif // __BINLOG_EVENT_FACTORY_H__


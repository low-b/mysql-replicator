#include "binlog_event_factory.h"
#include "start_event_v3.h"
#include "intvar_event.h"
#include "rotate_event.h"
#include "load_event.h"
#include "table_map_event.h"
#include "xid_event.h"
#include "query_event.h"
#include "format_description_event.h"
#include "mysql_replicator_com.h"

namespace mysql_replicator {
std::shared_ptr<LogEvent> BinlogEventFactory::create(std::shared_ptr<BinlogEventHeader> event_header) {
    std::shared_ptr<LogEvent> log_event;
    switch (event_header->event_type) {
        case log_event_type_t::START_EVENT_V3: {
            log_event = std::make_shared<StartEventV3>(event_header);
            break;
            }
        case log_event_type_t::QUERY_EVENT: {
            log_event = std::make_shared<QueryEvent>(event_header);
            break;
            }
        case log_event_type_t::ROTATE_EVENT: {
            log_event = std::make_shared<RotateEvent>(event_header);
            break;
            }
        case log_event_type_t::INTVAR_EVENT: {
            log_event = std::make_shared<IntvarEvent>(event_header);
            break;
            }
        case log_event_type_t::FORMAT_DESCRIPTION_EVENT: {
            log_event = std::make_shared<FormatDescriptionEvent>(event_header);
            break;
            }
        case log_event_type_t::XID_EVENT: {
            log_event = std::make_shared<XidEvent>(event_header);
            break;
            }
        case log_event_type_t::TABLE_MAP_EVENT: {
            log_event = std::make_shared<TableMapEvent>(event_header);
            break;
            }
        default:
            break;
    }
    return log_event;
}
}

#include "binlog_event_factory.h"
#include <istream>
#include "start_event_v3.h"
#include "intvar_event.h"
#include "rotate_event.h"
#include "load_event.h"
#include "table_map_event.h"
#include "xid_event.h"
#include "query_event.h"
#include "format_description_event.h"
#include "mysql_replicator_com.h"
#include "update_rows_event.h"
#include "write_rows_event.h"
#include "delete_rows_event.h"


namespace mysql_replicator {
std::shared_ptr<LogEvent> BinlogEventFactory::create(boost::asio::streambuf& packet_buf) {
    std::istream packet_stream(&packet_buf);
    std::shared_ptr<BinlogEventHeader> event_header = std::make_shared<BinlogEventHeader>();
    event_header->fromStream(packet_stream);
//    event_header->printPacket();
    std::shared_ptr<LogEvent> log_event;
    switch (event_header->event_type) {
        case log_event_type_t::START_EVENT_V3: {
            log_event = std::make_shared<StartEventV3>(event_header);
            log_event->fromStream(packet_stream);
            break;
        }
        case log_event_type_t::QUERY_EVENT: {
            log_event = std::make_shared<QueryEvent>(event_header);
            log_event->fromStream(packet_stream);
            break;
        }
        case log_event_type_t::ROTATE_EVENT: {
            log_event = std::make_shared<RotateEvent>(event_header);
            log_event->fromStream(packet_stream);
            break;
        }
        case log_event_type_t::INTVAR_EVENT: {
            log_event = std::make_shared<IntvarEvent>(event_header);
            log_event->fromStream(packet_stream);
            break;
        }
        case log_event_type_t::FORMAT_DESCRIPTION_EVENT: {
            std::shared_ptr<FormatDescriptionEvent> format_description_event
                = std::make_shared<FormatDescriptionEvent>(event_header);
            format_description_event->fromStream(packet_stream);
            format_description_event_ = format_description_event;
            log_event = format_description_event;
            break;
        }
        case log_event_type_t::XID_EVENT: {
            log_event = std::make_shared<XidEvent>(event_header);
            log_event->fromStream(packet_stream);
            break;
        }
        case log_event_type_t::TABLE_MAP_EVENT: {
            std::shared_ptr<TableMapEvent> table_map_event = std::make_shared<TableMapEvent>(event_header);
            table_map_event->fromStream(packet_stream);
            table_map_[table_map_event->get_table_id()] = table_map_event;
            log_event = table_map_event;
            break;
        }
        case log_event_type_t::UPDATE_ROWS_EVENT: {
            std::shared_ptr<UpdateRowsEvent> update_rows_event = std::make_shared<UpdateRowsEvent>(event_header,
                    format_description_event_->get_post_header_len(log_event_type_t::UPDATE_ROWS_EVENT));
            update_rows_event->fromStream(packet_stream);
            update_rows_event->set_table_map_event(table_map_);
            update_rows_event->set_db_meta(db_meta_);
            log_event = update_rows_event;
            break;
        }
        case log_event_type_t::WRITE_ROWS_EVENT: {
            std::shared_ptr<WriteRowsEvent> write_rows_event = std::make_shared<WriteRowsEvent>(event_header,
                    format_description_event_->get_post_header_len(log_event_type_t::WRITE_ROWS_EVENT));
            write_rows_event->fromStream(packet_stream);
            write_rows_event->set_table_map_event(table_map_);
            write_rows_event->set_db_meta(db_meta_);
            log_event = write_rows_event;
            break;
        }
        case log_event_type_t::DELETE_ROWS_EVENT: {
            std::shared_ptr<DeleteRowsEvent> delete_rows_event = std::make_shared<DeleteRowsEvent>(event_header,
                    format_description_event_->get_post_header_len(log_event_type_t::DELETE_ROWS_EVENT));
            delete_rows_event->fromStream(packet_stream);
            delete_rows_event->set_table_map_event(table_map_);
            delete_rows_event->set_db_meta(db_meta_);
            log_event = delete_rows_event;
            break;
        }
        default:
            return std::shared_ptr<LogEvent>();
    }
//    log_event->printPacket();
    return log_event;
}
}

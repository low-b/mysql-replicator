#include "mysql_replicator_dumper.h"
#include "packet_header.h"
#include "eof_packet.h"
#include "err_packet.h"
#include "binlog_event_header.h"
#include "bytes_helper.h"
#include "network_exception.h"
#include "binlog_dump_exception.h"
#include "start_event_v3.h"
#include "intvar_event.h"
#include "rotate_event.h"
#include "load_event.h"
#include "table_map_event.h"
#include "xid_event.h"
#include "query_event.h"
#include "format_description_event.h"

namespace mysql_replicator {

void MySQLReplicatorDumper::sendBinlogDump(std::shared_ptr<BinlogCheckPoint> binlog_pos) {
    //send binlog dump packet
    BytesHelper::write(socket_, binlog_pos->get_dump_packet());
}

std::shared_ptr<LogEvent> MySQLReplicatorDumper::takeBinlogEvent() {
    //get packet header
    std::shared_ptr<PacketHeader> response_header(new PacketHeader);
    BytesHelper::readHeader(socket_, response_header);
    //get binlog event header
    boost::asio::streambuf response_message;
    std::istream response_stream(&response_message);
    boost::asio::read(*socket_, response_message,
            boost::asio::transfer_exactly(response_header->get_packet_len()));
    uint8_t stream_status = response_stream.peek();
    if (!stream_status == 0) {
        if (stream_status == 254) {
            EofPacket eof_packet;
            eof_packet.fromStream(response_stream);
            eof_packet.printPacket();
            return std::shared_ptr<LogEvent>();
        } else if (stream_status == 255) {
            ErrPacket err_packet;
            err_packet.fromStream(response_stream);
            err_packet.printPacket();
            throw BinlogDumpException("error occurred when take event from master");
        } else {
            throw BinlogDumpException("unknown return code");
        }
    }
    std::shared_ptr<BinlogEventHeader> event_header = std::make_shared<BinlogEventHeader>();
    event_header->fromStream(response_stream);
    event_header->printPacket();

    //if (handle_set_.find(event_header->event_type) == handle_set_.end()) {
    //    return std::shared_ptr<LogEvent>();
    //}

    std::shared_ptr<LogEvent> log_event;
    switch (event_header->event_type) {
        case log_event_type_t::START_EVENT_V3: {
            log_event = std::make_shared<StartEventV3>(event_header);
            log_event->fromStream(response_stream);
            break;
            }
        case log_event_type_t::QUERY_EVENT: {
            log_event = std::make_shared<QueryEvent>(event_header);
            log_event->fromStream(response_stream);
            break;
            }
        case log_event_type_t::ROTATE_EVENT: {
            log_event = std::make_shared<RotateEvent>(event_header);
            log_event->fromStream(response_stream);
            break;
            }
        case log_event_type_t::INTVAR_EVENT: {
            log_event = std::make_shared<IntvarEvent>(event_header);
            log_event->fromStream(response_stream);
            break;
            }
        case log_event_type_t::FORMAT_DESCRIPTION_EVENT: {
            log_event = std::make_shared<FormatDescriptionEvent>(event_header);
            log_event->fromStream(response_stream);
            break;
            }
        case log_event_type_t::XID_EVENT: {
            log_event = std::make_shared<XidEvent>(event_header);
            log_event->fromStream(response_stream);
            break;
            }
        case log_event_type_t::TABLE_MAP_EVENT: {
            log_event = std::make_shared<TableMapEvent>(event_header);
            log_event->fromStream(response_stream);
            break;
            }
        default:
            break;
    
    }
    if (log_event) {
        log_event->printPacket();
    }
    
    return std::shared_ptr<LogEvent>();
}

}

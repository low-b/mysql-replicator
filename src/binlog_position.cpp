#include "binlog_position.h"
#include "binlog_dump_packet.h"
#include "binlog_event_header.h"
#include "mysql_replicator_com.h"
#include "rotate_event.h"

using std::shared_ptr;

namespace mysql_replicator {
shared_ptr<Packet> BinlogPosition::get_dump_packet() {
    shared_ptr<BinlogDumpPacket> dump_packet(new BinlogDumpPacket);
    dump_packet->set_binlog_filename(binlog_filename_);
    dump_packet->set_binlog_pos(binlog_pos_);
    dump_packet->set_flags(0);
    dump_packet->set_slave_id(slave_id_);
    return dump_packet;
}
void BinlogPosition::process(std::shared_ptr<LogEvent> log_event) {
    shared_ptr<BinlogEventHeader> header = log_event->get_header();
    if (header->log_pos != 0) {
        binlog_pos_ = header->log_pos;
    }
    switch (header->event_type) {
        case log_event_type_t::ROTATE_EVENT: {
            shared_ptr<RotateEvent> rotate_event = std::dynamic_pointer_cast<RotateEvent>(log_event);
            binlog_filename_ = rotate_event->get_next_binlog_name();
            binlog_pos_ = rotate_event->get_position();
        }
    }
}
}

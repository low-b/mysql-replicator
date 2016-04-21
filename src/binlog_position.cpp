#include "binlog_position.h"
#include "binlog_dump_packet.h"

namespace mysql_replicator {
std::shared_ptr<Packet> BinlogPosition::get_dump_packet() {
    std::shared_ptr<BinlogDumpPacket> dump_packet(new BinlogDumpPacket);
    dump_packet->set_binlog_filename(binlog_filename_);
    dump_packet->set_binlog_pos(binlog_pos_);
    dump_packet->set_flags(0);
    dump_packet->set_slave_id(slave_id_);
    return dump_packet;
}
}

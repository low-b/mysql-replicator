#include "binlog_dump_packet.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void BinlogDumpPacket::toStream(std::ostream &os) {
    BytesHelper::writeFixUint8(os, command_);
    BytesHelper::writeFixUint32(os, binlog_pos_);
    BytesHelper::writeFixUint16(os, flags_);
    BytesHelper::writeFixUint32(os, slave_id_);
    BytesHelper::writeNullString(os, binlog_filename_);
}

void BinlogDumpPacket::printPacket() {
    std::cout << "--------binlog dump packet begin---------" << std::endl;
    std::cout << "command_:" << (uint32_t)command_ << std::endl;
    std::cout << "binlog_pos_:" << binlog_pos_ << std::endl;
    std::cout << "flags_:" << flags_ << std::endl;
    std::cout << "slave_id_:" << slave_id_ << std::endl;
    std::cout << "binlog_filename_:" << binlog_filename_ << std::endl;
    std::cout << "--------binlog dump packet end---------" << std::endl;
}
}

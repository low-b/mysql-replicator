#include "binlog_event_header.h"
#include "bytes_helper.h"

namespace mysql_replicator {
void BinlogEventHeader::fromStream(std::istream &is) {
    BytesHelper::readFixUint8(is, marker);
    BytesHelper::readFixUint32(is, timestamp);
    BytesHelper::readFixUint8(is, event_type);
    BytesHelper::readFixUint32(is, server_id);
    BytesHelper::readFixUint32(is, event_size);
    BytesHelper::readFixUint32(is, log_pos);
    BytesHelper::readFixUint16(is, flags);
}
void BinlogEventHeader::printPacket() {
    std::cout << "--------packet header begin---------" << std::endl;
    std::cout << "marker_:" << (uint32_t)marker << std::endl;
    std::cout << "timestamp_:" << (uint32_t)timestamp << std::endl;
    std::cout << "event_type_:" << (uint32_t)event_type << std::endl;
    std::cout << "server_id_:" << (uint32_t)server_id << std::endl;
    std::cout << "event_size_:" << (uint32_t)event_size << std::endl;
    std::cout << "log_pos_:" << (uint32_t)log_pos << std::endl;
    std::cout << "flags_:" << (uint32_t)flags << std::endl;
    std::cout << "--------packet header end---------" << std::endl;
}
}

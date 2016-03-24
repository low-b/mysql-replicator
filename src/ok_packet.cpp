#include "ok_packet.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void OkPacket::fromStream(std::istream &is) {
    BytesHelper::readFixUint8(is, header_);
    BytesHelper::readLenencUint(is, affected_rows_);
    BytesHelper::readLenencUint(is, last_insert_id_);
    BytesHelper::readFixUint16(is, status_flags_);
    BytesHelper::readNullString(is, status_info_);
}

void OkPacket::printPacket() {
    std::cout << "--------ok packet begin---------" << std::endl;
    std::cout << "header_:" << (uint32_t)header_ << std::endl;
    std::cout << "affected_rows_:" << affected_rows_ << std::endl;
    std::cout << "last_insert_id_:" << last_insert_id_ << std::endl;
    std::cout << "status_flags_:" << status_flags_ << std::endl;
    std::cout << "status_info_:" << status_info_ << std::endl;
    std::cout << "--------ok packet end---------" << std::endl;
}
}

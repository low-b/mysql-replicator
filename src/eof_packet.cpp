#include "eof_packet.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void EofPacket::fromStream(std::istream &is) {
    BytesHelper::readFixUint8(is, header_);
    BytesHelper::readFixUint16(is, warnings_num_);
    BytesHelper::readFixUint16(is, status_flags_);
}

void EofPacket::printPacket() {
    std::cout << "--------eof packet begin---------" << std::endl;
    std::cout << "header_:" << (uint32_t)header_ << std::endl;
    std::cout << "warnings_num_:" << warnings_num_ << std::endl;
    std::cout << "status_flags_:" << status_flags_ << std::endl;
    std::cout << "--------eof packet end---------" << std::endl;
}
}

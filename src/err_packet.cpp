#include "err_packet.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void ErrPacket::fromStream(std::istream &is) {
    BytesHelper::readFixUint8(is, header_);
    BytesHelper::readFixUint16(is, error_code_);
    BytesHelper::readFixString(is, sql_state_marker_, 1);
    BytesHelper::readFixString(is, sql_state_, 5);
    BytesHelper::readNullString(is, error_message_);
}

void ErrPacket::printPacket() {
    std::cout << "--------err packet begin---------" << std::endl;
    std::cout << "header_:" << (uint32_t)header_ << std::endl;
    std::cout << "error_code_:" << error_code_ << std::endl;
    std::cout << "sql_state_marker_:" << sql_state_marker_ << std::endl;
    std::cout << "sql_state_:" << sql_state_ << std::endl;
    std::cout << "error_message_:" << error_message_ << std::endl;
    std::cout << "--------err packet end---------" << std::endl;
}
}

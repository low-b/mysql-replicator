#include "rotate_event.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void RotateEvent::fromStream(std::istream &is) {
    BytesHelper::readFixUint64(is, position_);
    BytesHelper::readNullString(is, next_binlog_name_);
}

void RotateEvent::printPacket() {
    std::cout << "--------rotate event packet begin---------" << std::endl;
    std::cout << "position_:" << position_ << std::endl;
    std::cout << "next_binlog_name_:" << next_binlog_name_ << std::endl;
    std::cout << "--------rotate event packet end---------" << std::endl;
}
}

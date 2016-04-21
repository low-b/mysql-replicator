#include "intvar_event.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void IntvarEvent::fromStream(std::istream &is) {
    BytesHelper::readFixUint8(is, type_);
    BytesHelper::readFixUint64(is, value_);
}

void IntvarEvent::printPacket() {
    std::cout << "--------intvar event packet begin---------" << std::endl;
    std::cout << "type_:" << (uint32_t)type_ << std::endl;
    std::cout << "value_:" << value_ << std::endl;
    std::cout << "--------intvar event packet end---------" << std::endl;
}
}

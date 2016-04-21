#include "xid_event.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void XidEvent::fromStream(std::istream &is) {
    BytesHelper::readFixUint8(is, xid_);
}

void XidEvent::printPacket() {
    std::cout << "--------xid event packet begin---------" << std::endl;
    std::cout << "xid_:" << (uint32_t)xid_ << std::endl;
    std::cout << "--------xid event packet end---------" << std::endl;
}
}

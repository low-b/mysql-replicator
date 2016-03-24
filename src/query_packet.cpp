#include "query_packet.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void QueryPacket::toStream(std::ostream &os) {
    BytesHelper::writeFixUint8(os, command_);
    BytesHelper::writeFixString(os, query_string_, query_string_.length());
}

void QueryPacket::printPacket() {
    std::cout << "--------query packet begin---------" << std::endl;
    std::cout << "command_:" << (uint32_t)command_ << std::endl;
    std::cout << "query_string_:" << query_string_ << std::endl;
    std::cout << "--------query packet end---------" << std::endl;
}
}

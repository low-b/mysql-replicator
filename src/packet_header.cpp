#include "packet_header.h"
#include "bytes_helper.h"

namespace mysql_replicator {
void PacketHeader::fromStream(std::istream &is) {
    BytesHelper::readFixUint24(is, packet_len_);
    BytesHelper::readFixUint8(is, seq_num_);
}
void PacketHeader::toStream(std::ostream &os) {
    BytesHelper::writeFixUint24(os, packet_len_);
    BytesHelper::writeFixUint8(os, seq_num_);
}
void PacketHeader::printPacket() {
    std::cout << "--------packet header begin---------" << std::endl;
    std::cout << "packet_len_:" << (uint32_t)packet_len_ << std::endl;
    std::cout << "seq_num_:" << (uint32_t)seq_num_ << std::endl;
    std::cout << "--------packet header end---------" << std::endl;
}
}

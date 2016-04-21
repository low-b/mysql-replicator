#include "resultset_row_packet.h"
#include "bytes_helper.h"

namespace mysql_replicator {
void ResultsetRowPacket::fromStream(std::istream &is) {
    for (uint64_t i = 0; i < column_count_; ++i) {
        std::string data;
        BytesHelper::readLenencString(is, data);
        data_vec_.push_back(data);
    }
}
void ResultsetRowPacket::printPacket() {
    std::cout << "--------resultset row packet begin---------" << std::endl;
    for (uint64_t i = 0; i < column_count_; ++i) {
        std::cout << "data_:" << data_vec_[i].data() << std::endl;
    }
    std::cout << "--------resultset row packet end---------" << std::endl;
}
}

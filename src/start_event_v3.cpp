#include "start_event_v3.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void StartEventV3::fromStream(std::istream &is) {
    BytesHelper::readFixUint16(is, binlog_version_);
    BytesHelper::readFixString(is, mysql_server_version_, 50);
    BytesHelper::readFixUint32(is, create_timestamp_);
}

void StartEventV3::printPacket() {
    std::cout << "--------start event v3 packet begin---------" << std::endl;
    std::cout << "binlog_version_:" << binlog_version_ << std::endl;
    std::cout << "mysql_server_version_:" << mysql_server_version_ << std::endl;
    std::cout << "create_timestamp_:" << create_timestamp_ << std::endl;
    std::cout << "--------start event v3 packet end---------" << std::endl;
}
}

#include "format_description_event.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void FormatDescriptionEvent::fromStream(std::istream &is) {
    BytesHelper::readFixUint16(is, binlog_version_);
    BytesHelper::readFixString(is, mysql_server_version_, 50);
    BytesHelper::readFixUint32(is, create_timestamp_);
    BytesHelper::readFixUint8(is, event_header_len_);
    std::string event_type_header_lens_string;
    BytesHelper::readEofString(is, event_type_header_lens_string);
    for (size_t i = 0; i < event_type_header_lens_string.length(); ++i) {
        event_type_header_lens_.push_back(event_type_header_lens_string[i]);
    }
}

void FormatDescriptionEvent::printPacket() {
    std::cout << "--------format_description event packet begin---------" << std::endl;
    std::cout << "binlog_version_:" << binlog_version_ << std::endl;
    std::cout << "mysql_server_version_:" << mysql_server_version_ << std::endl;
    std::cout << "create_timestamp_:" << create_timestamp_ << std::endl;
    std::cout << "event_header_len_:" << (uint32_t)event_header_len_ << std::endl;
    for (size_t i = 0; i < event_type_header_lens_.size(); ++i) {
        std::cout << "event_type_header_lens_[" << i << "]:" << (uint32_t)event_type_header_lens_[i] << std::endl;
    }
    std::cout << "--------format_description event packet end---------" << std::endl;
}
}

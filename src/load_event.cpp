#include "load_event.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void LoadEvent::fromStream(std::istream &is) {
    BytesHelper::readFixUint32(is, slave_proxy_id_);
    BytesHelper::readFixUint32(is, exec_time_);
    BytesHelper::readFixUint32(is, skip_lines_);
    BytesHelper::readFixUint8(is, table_name_len_);
    BytesHelper::readFixUint8(is, schema_name_len_);
    BytesHelper::readFixUint32(is, num_fields_);
    BytesHelper::readFixUint8(is, field_term_);
    BytesHelper::readFixUint8(is, enclosed_by_);
    BytesHelper::readFixUint8(is, line_term_);
    BytesHelper::readFixUint8(is, line_start_);
    BytesHelper::readFixUint8(is, escaped_by_);
    BytesHelper::readFixUint8(is, opt_flags_);
    BytesHelper::readFixUint8(is, empty_flags_);
    for (size_t i = 0; i < num_fields_; ++i) {
        uint8_t num_field_len;
        BytesHelper::readFixUint8(is, num_field_len);
        field_name_lens_.push_back(num_field_len);
    }
    for (size_t i = 0; i < num_fields_; ++i) {
        std::string field_name;
        BytesHelper::readFixString(is, field_name, field_name_lens_[i]);
        field_names_.push_back(std::move(field_name));
    }
    BytesHelper::readFixString(is, table_name_, table_name_len_);
    BytesHelper::readFixString(is, schema_name_, schema_name_len_);
    BytesHelper::readNullString(is, file_name_);
}

void LoadEvent::printPacket() {
    std::cout << "--------load event packet begin---------" << std::endl;
    std::cout << "slave_proxy_id_:" << slave_proxy_id_ << std::endl;
    std::cout << "exec_time_:" << exec_time_ << std::endl;
    std::cout << "skip_lines_:" << skip_lines_ << std::endl;
    std::cout << "table_name_len_:" << (uint32_t)table_name_len_ << std::endl;
    std::cout << "schema_name_len_:" << (uint32_t)schema_name_len_ << std::endl;
    std::cout << "num_fields_:" << num_fields_ << std::endl;
    std::cout << "field_term_:" << (uint32_t)field_term_ << std::endl;
    std::cout << "enclosed_by_:" << (uint32_t)enclosed_by_ << std::endl;
    std::cout << "line_term_:" << (uint32_t)line_term_ << std::endl;
    std::cout << "line_start_:" << (uint32_t)line_start_ << std::endl;
    std::cout << "escaped_by_:" << (uint32_t)escaped_by_ << std::endl;
    std::cout << "opt_flags_:" << (uint32_t)opt_flags_ << std::endl;
    std::cout << "empty_flags_:" << (uint32_t)empty_flags_ << std::endl;
    for (size_t i = 0; i < num_fields_; ++i) {
        std::cout << "field_len[:" << i << "]" << field_name_lens_[i] << std::endl;
        std::cout << "field[:" << i << "]" << field_names_[i] << std::endl;
    }
    std::cout << "table_name_:" << table_name_ << std::endl;
    std::cout << "schema_name_:" << schema_name_ << std::endl;
    std::cout << "file_name_:" << file_name_ << std::endl;
    std::cout << "--------load event packet end---------" << std::endl;
}
}

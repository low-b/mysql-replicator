#include "query_event.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void QueryEvent::fromStream(std::istream &is) {
    BytesHelper::readFixUint32(is, slave_proxy_id_);
    BytesHelper::readFixUint32(is, exec_time_);
    BytesHelper::readFixUint8(is, schema_len_);
    BytesHelper::readFixUint16(is, error_code_);
    BytesHelper::readFixUint16(is, status_vars_len_);
    BytesHelper::readFixString(is, status_vars_, status_vars_len_);
    BytesHelper::readFixString(is, schema_, schema_len_);
    BytesHelper::readFixUint8(is, fill_);
    BytesHelper::readNullString(is, query_);
}

void QueryEvent::printPacket() {
    std::cout << "--------query event packet begin---------" << std::endl;
    std::cout << "slave_proxy_id_:" << slave_proxy_id_ << std::endl;
    std::cout << "execution_time_:" << exec_time_ << std::endl;
    std::cout << "schema_length_:" << (uint32_t)schema_len_ << std::endl;
    std::cout << "error_code_:" << error_code_ << std::endl;
    std::cout << "status_vars_length_:" << status_vars_len_ << std::endl;
    std::cout << "status_vars_:" << status_vars_ << std::endl;
    std::cout << "schema_:" << schema_ << std::endl;
    std::cout << "query_:" << query_ << std::endl;
    std::cout << "--------query event packet end---------" << std::endl;
}
}

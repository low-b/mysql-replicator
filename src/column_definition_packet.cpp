#include "column_definition_packet.h"
#include "bytes_helper.h"

using std::shared_ptr;
namespace mysql_replicator {

void ColumnDefinitionPacket::fromStream(std::istream &is) {
    BytesHelper::readLenencString(is, catalog_);
    BytesHelper::readLenencString(is, schema_);
    BytesHelper::readLenencString(is, table_);
    BytesHelper::readLenencString(is, org_table_);
    BytesHelper::readLenencString(is, name_);
    BytesHelper::readLenencString(is, org_name_);
    BytesHelper::readFixUint8(is, next_length_);
    BytesHelper::readFixUint16(is, charset_);
    BytesHelper::readFixUint32(is, column_len_);
    BytesHelper::readFixUint8(is, type_);
    BytesHelper::readFixUint16(is, flags_);
    BytesHelper::readFixUint8(is, decimals_);
    BytesHelper::readFixUint16(is, filler_);
}
void ColumnDefinitionPacket::printPacket() {
    std::cout << "--------column definition packet begin---------" << std::endl;
    std::cout << "catalog_:" << catalog_ << std::endl;
    std::cout << "schema_:" << schema_ << std::endl;
    std::cout << "table_:" << table_ << std::endl;
    std::cout << "org_table_:" << org_table_ << std::endl;
    std::cout << "name_:" << name_ << std::endl;
    std::cout << "org_name_:" << org_name_ << std::endl;
    std::cout << "next_length_:" << (uint32_t)next_length_ << std::endl;
    std::cout << "charset_:" << charset_ << std::endl;
    std::cout << "column_length_:" << column_len_ << std::endl;
    std::cout << "type_:" << (uint32_t)type_ << std::endl;
    std::cout << "flags_:" << flags_ << std::endl;
    std::cout << "decimals_:" << (uint32_t)decimals_ << std::endl;
    std::cout << "filler_:" << filler_ << std::endl;
    std::cout << "--------column definition packet end---------" << std::endl;
}
}

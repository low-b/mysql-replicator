#include "table_map_event.h"
#include "bytes_helper.h"
#include "mysql_replicator_com.h"

namespace mysql_replicator {

void TableMapEvent::fromStream(std::istream &is) {
    BytesHelper::readFixUint48(is, table_id_);
    BytesHelper::readFixUint16(is, flags_);
    BytesHelper::readFixUint8(is, schema_name_len_);
    BytesHelper::readFixString(is, schema_name_, schema_name_len_);
    BytesHelper::readFixUint8(is, fill1_);
    BytesHelper::readFixUint8(is, table_name_len_);
    BytesHelper::readFixString(is, table_name_, table_name_len_);
    BytesHelper::readFixUint8(is, fill2_);
    BytesHelper::readLenencUint(is, column_count_);
    for (size_t i = 0; i < column_count_; ++i) {
        uint8_t tmp_type;
        BytesHelper::readFixUint8(is, tmp_type);
        column_type_def_.push_back(tmp_type);
    }
    for (size_t i = 0; i < column_count_; ++i) {
        switch (column_type_def_[i]) {
            case MYSQL_TYPE_TINY_BLOB:
            case MYSQL_TYPE_BLOB:
            case MYSQL_TYPE_MEDIUM_BLOB:
            case MYSQL_TYPE_LONG_BLOB:
            case MYSQL_TYPE_DOUBLE:
            case MYSQL_TYPE_FLOAT:
            case MYSQL_TYPE_GEOMETRY:
            case MYSQL_TYPE_TIME2:
            case MYSQL_TYPE_DATETIME2:
            case MYSQL_TYPE_TIMESTAMP2:
            case MYSQL_TYPE_JSON: {
                uint8_t tmp_meta;
                BytesHelper::readFixUint8(is, tmp_meta);
                column_meta_def_.push_back(tmp_meta);
                break;
            }
            case MYSQL_TYPE_SET:
            case MYSQL_TYPE_ENUM:
            case MYSQL_TYPE_NEWDECIMAL:
            case MYSQL_TYPE_STRING: {
                uint16_t tmp_meta;
                uint8_t tmp;
                BytesHelper::readFixUint8(is, tmp);
                tmp_meta = tmp << 8;
                BytesHelper::readFixUint8(is, tmp);
                tmp_meta += tmp;
                column_meta_def_.push_back(tmp_meta);
                break;
            }
            case MYSQL_TYPE_BIT:
            case MYSQL_TYPE_VARCHAR: {
                uint16_t tmp_meta;
                BytesHelper::readFixUint16(is, tmp_meta);
                column_meta_def_.push_back(tmp_meta);
                break;

            }
            default: {
                column_meta_def_.push_back(0);
                break;
            }

        }
    }
    size_t null_bitsmap_size = (column_count_ + 7) / 8;
    for (size_t i = 0; i < null_bitsmap_size; ++i) {
        uint8_t tmp;
        BytesHelper::readFixUint8(is, tmp);
        null_bitmap_.push_back(tmp);
    }
}

void TableMapEvent::printPacket() {
    std::cout << "--------table map event packet begin---------" << std::endl;
    std::cout << "--------table map event packet end---------" << std::endl;
}
}

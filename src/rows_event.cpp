#include "rows_event.h"
#include <sstream>
#include "bytes_helper.h"
#include "mysql_replicator_com.h"
#include "binlog_event_exception.h"
#include "mysql_util.h"
#include "m_string.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::istream;
using std::to_string;

#include <iostream>

using namespace std;

namespace mysql_replicator {

void RowsEvent::fromStream(std::istream &is) {
    if (post_header_len_ == 6) {
        uint32_t tmp_table_id = 0;
        BytesHelper::readFixUint32(is, tmp_table_id);
        table_id_ = tmp_table_id;
    } else {
        BytesHelper::readFixUint48(is, table_id_);
    }
    BytesHelper::readFixUint16(is, flags_);
    if (post_header_len_ == 10) {
        BytesHelper::readFixUint16(is, extra_data_len_);
        BytesHelper::readFixString(is, extra_data_, extra_data_len_);
    }
    BytesHelper::readLenencUint(is, column_count_);
    size_t columns_bitset_size = (column_count_ + 7) / 8;
    BytesHelper::readBitmap(is, columns_before_set_, columns_bitset_size);
    std::shared_ptr<BinlogEventHeader> header = get_header();
    if (header->event_type == log_event_type_t::UPDATE_ROWS_EVENT
            || header->event_type == log_event_type_t::UPDATE_ROWS_EVENTv2) {
        BytesHelper::readBitmap(is, columns_after_set_, columns_bitset_size);
    }
    BytesHelper::readEofString(is, row_data_);
}

void RowsEvent::buildEventProto(std::vector<std::shared_ptr<BinlogEvent> >& event_vec) {
    shared_ptr<BinlogEvent> binlog_event_prototype(new BinlogEvent);

    if (!table_map_event_) {
        return;
    }
    string schema_name = table_map_event_->get_schema_name();
    string table_name = table_map_event_->get_table_name();
    binlog_event_prototype->set_db(schema_name);
    binlog_event_prototype->set_table(table_name);

    std::shared_ptr<BinlogEventHeader> header = get_header();
    if (header->event_type == log_event_type_t::UPDATE_ROWS_EVENT
            || header->event_type == log_event_type_t::UPDATE_ROWS_EVENTv2) {
        binlog_event_prototype->set_event_type(UPDATE_EVENT);
    } else if (header->event_type == log_event_type_t::WRITE_ROWS_EVENT
            || header->event_type == log_event_type_t::WRITE_ROWS_EVENTv2) {
        binlog_event_prototype->set_event_type(INSERT_EVENT);
    } else if (header->event_type == log_event_type_t::DELETE_ROWS_EVENT
            || header->event_type == log_event_type_t::DELETE_ROWS_EVENTv2) {
        binlog_event_prototype->set_event_type(DELETE_EVENT);
    }

//    Status *status = binlog_event_prototype->mutable_status();

    std::shared_ptr<TableMeta> table_meta = db_meta_->get_table(schema_name, table_name);
    if (!table_meta) {
        return;
    }
    const vector<uint8_t>& column_type_def = table_map_event_->get_column_type_def();
    const vector<uint16_t>& column_meta_def = table_map_event_->get_column_meta_def();

    size_t total_size = row_data_.size();
    size_t current_size = 0;
    std::istringstream is(row_data_);
    while (current_size < total_size) {
        shared_ptr<BinlogEvent> binlog_event(new BinlogEvent);
        binlog_event->CopyFrom(*binlog_event_prototype);
        Row* row = binlog_event->mutable_row();
        RowsEvent::buildRowProto(is, row, table_meta, column_type_def, column_meta_def, current_size, true);
        if (header->event_type == log_event_type_t::UPDATE_ROWS_EVENT
                || header->event_type == log_event_type_t::UPDATE_ROWS_EVENTv2) {
            RowsEvent::buildRowProto(is, row, table_meta, column_type_def, column_meta_def, current_size, false);
        }
        event_vec.push_back(binlog_event);
    }

}
void RowsEvent::buildRowProto(std::istream& is,
            Row* row,
            std::shared_ptr<TableMeta> table_meta,
            const vector<uint8_t>& column_type_def,
            const vector<uint16_t>& column_meta_def,
            size_t& current_size,
            bool is_new) {
    boost::dynamic_bitset<> columns_null_set;
    size_t null_bitset_size = (columns_before_set_.count() + 7) / 8;
    BytesHelper::readBitmap(is, columns_null_set, null_bitset_size);
    current_size +=null_bitset_size;
    size_t null_index = 0;
    uint32_t field_type = 0;
    uint32_t field_len = 0;
    for (size_t i = 0; i < column_count_; ++i) {

        if (!columns_before_set_[i]) {
            continue;
        }
        std::shared_ptr<ColumnMeta> col_meta = table_meta->at(i);
        Field *field;
        MysqlType column_type = static_cast<MysqlType>(column_type_def[i]);
        field_type = column_type_def[i];
        uint16_t meta = column_meta_def[i];
        bool is_signed = col_meta->get_is_signed();
        string value;
        if (is_new) {
            field = row->add_field();
            field->set_name(col_meta->get_column_name());
            field->set_mysql_type(column_type);
            field->set_is_signed(is_signed);
            field->set_is_pk(col_meta->get_is_pk());
        } else {
            field = row->mutable_field(i);
        }
        if (columns_null_set[null_index]){
            if (is_new) {
                field->set_is_new_null(true);
            } else {
                field->set_is_old_null(true);
            }
        } else {
            if (column_type == MYSQL_TYPE_STRING) {
                if (meta >= 256) {
                    uint32_t byte0 = meta >> 8;
                    uint32_t byte1 = meta && 0xff;
                    if ((byte0 & 0x30) != 0x30) {
                        field_len = byte1 | (((byte0 & 0x30) ^ 0x30) << 4);
                        field_type = byte0 | 0x30;
                    } else {
                        switch (byte0) {
                            case MYSQL_TYPE_SET:
                            case MYSQL_TYPE_ENUM:
                            case MYSQL_TYPE_STRING:
                                field_type = byte0;
                                field_len = byte1;
                                break;

                            default:
                                throw BinlogEventException("Illegal event");
                        }
                    }
                } else {
                    field_len = meta;
                }
            }
            switch (field_type) {
                case MYSQL_TYPE_LONG: {
                    if (is_signed) {
                        int32_t si = 0;
                        BytesHelper::readFixInt32(is, si);
                        value = to_string(si);
                    } else {
                        uint32_t ui = 0;
                        BytesHelper::readFixUint32(is, ui);
                        value = to_string(ui);
                    }
                    current_size += 4;
                    break;
                }

                case MYSQL_TYPE_TINY: {
                    if (is_signed) {
                        int8_t si = 0;
                        BytesHelper::readFixInt8(is, si);
                        value = to_string(si);
                    } else {
                        uint8_t ui = 0;
                        BytesHelper::readFixUint8(is, ui);
                        value = to_string(ui);
                    }
                    current_size += 1;
                    break;
                }

                case MYSQL_TYPE_SHORT: {
                    if (is_signed) {
                        int16_t si = 0;
                        BytesHelper::readFixInt16(is, si);
                        value = to_string(si);
                    } else {
                        uint16_t ui = 0;
                        BytesHelper::readFixUint16(is, ui);
                        value = to_string(ui);
                    }
                    current_size += 2;
                    break;
                }

                case MYSQL_TYPE_INT24: {
                    if (is_signed) {
                        int32_t si = 0;
                        BytesHelper::readFixInt24(is, si);
                        value = to_string(si);
                    } else {
                        uint32_t ui = 0;
                        BytesHelper::readFixUint24(is, ui);
                        value = to_string(ui);
                    }
                    current_size += 3;
                    break;
                }

                case MYSQL_TYPE_LONGLONG: {
                    if (is_signed) {
                        int64_t si = 0;
                        BytesHelper::readFixInt64(is, si);
                        value = to_string(si);
                    } else {
                        uint64_t ui = 0;
                        BytesHelper::readFixUint64(is, ui);
                        value = to_string(ui);
                    }
                    current_size += 8;
                    break;
                }

                case MYSQL_TYPE_NEWDECIMAL: {
                    uint precision= meta >> 8;
                    uint decimals= meta & 0xFF;
                    uint bin_size= my_decimal_get_binary_size(precision, decimals);
                    string decimal_str;
                    BytesHelper::readFixString(is, decimal_str, bin_size);
                    current_size += bin_size;
                    uchar *ptr = (uchar*)decimal_str.data();
                    decimal_t dec;
                    bin2decimal(ptr, &dec, precision, decimals);
                    int len= DECIMAL_MAX_STR_LENGTH;
                    char buff[DECIMAL_MAX_STR_LENGTH + 1];
                    decimal2string(&dec,buff,&len, 0, 0, 0);
                    value = buff;
                    break;
                }

                case MYSQL_TYPE_FLOAT: {
                    float fl = 0.0;
                    BytesHelper::readFixFloat(is, fl);
                    current_size += sizeof(fl);
                    char float_buff[MY_GCVT_MAX_FIELD_WIDTH];
                    size_t write_len = my_gcvt((double) fl, MY_GCVT_ARG_FLOAT,
                            sizeof(float_buff) - 1, float_buff, NULL);
                    value.assign(float_buff, write_len);
                    break;
                }

                case MYSQL_TYPE_DOUBLE: {
                    double dbl = 0.0;
                    BytesHelper::readFixDouble(is, dbl);
                    current_size += sizeof(dbl);
                    char double_buff[MY_GCVT_MAX_FIELD_WIDTH];
                    size_t write_len = my_gcvt(dbl, MY_GCVT_ARG_DOUBLE,
                            sizeof(double_buff) - 1, double_buff, NULL);
                    value.assign(double_buff, write_len);
                    break;
                }

                case MYSQL_TYPE_BIT: {
                    uint32_t nbits = ((meta >> 8) * 8) + (meta & 0xff);
                    size_t size = (nbits + 7) / 8;
                    boost::dynamic_bitset<> my_bitset;
                    BytesHelper::readBitmap(is, my_bitset, size);
                    my_bitset.resize(nbits);
                    current_size += size;
                    boost::to_string(my_bitset, value);
                    break;
                }

                case MYSQL_TYPE_TIMESTAMP: {
                    uint32_t tmp_time = 0;
                    BytesHelper::readFixUint32(is, tmp_time);
                    current_size += 4;
                    value = to_string(tmp_time);
                    break;
                }

                case MYSQL_TYPE_DATETIME: {
                    size_t d, t;
                    uint64_t i64= 0;
                    BytesHelper::readFixUint64(is, i64);
                    current_size += 8;
                    d= static_cast<size_t>(i64 / 1000000);
                    t= i64 % 1000000;
                    char time_buff[20];
                    sprintf(time_buff, "%04d-%02d-%02d %02d:%02d:%02d",
                            static_cast<int>(d / 10000),
                            static_cast<int>(d % 10000) / 100,
                            static_cast<int>(d % 100),
                            static_cast<int>(t / 10000),
                            static_cast<int>(t % 10000) / 100,
                            static_cast<int>(t % 100));
                    value = time_buff;
                    break;
                }
                case MYSQL_TYPE_DATETIME2: {
                    string time_str;
                    BytesHelper::readFixString(is, time_str, meta);
                    current_size += meta;
                    uchar *ptr = (uchar*)time_str.data();
                    char buf[MAX_DATE_STRING_REP_LENGTH];
                    MYSQL_TIME ltime;
                    longlong packed= my_datetime_packed_from_binary(ptr, meta);
                    TIME_from_longlong_datetime_packed(&ltime, packed);
                    int buflen= my_datetime_to_str(&ltime, buf, meta);
                    value.assign(buf, buflen);
                    break;
               }

                case MYSQL_TYPE_TIME: {
                    uint32_t i24= 0;
                    BytesHelper::readFixUint24(is, i24);
                    current_size += 3;
                    char time_buff[9];
                    time_buff[8] = '\0';
                    sprintf(time_buff, "%02d:%02d:%02d",
                            static_cast<int>(i24 / 10000),
                            static_cast<int>(i24 % 10000) / 100,
                            static_cast<int>(i24 % 100));
                    value = time_buff;
                    break;
                }
                case MYSQL_TYPE_DATE: {
                    uint32_t i24= 0;
                    BytesHelper::readFixUint24(is, i24);
                    current_size += 3;
                    char time_buff[11];
                    time_buff[10] = '\0';
                    sprintf(time_buff, "%04d-%02d-%02d",
                            static_cast<int>((i24 >> 9) & 0x7fff),
                            static_cast<int>((i24 >> 5) & 0xf),
                            static_cast<int>(i24 & 0x1f));
                    value = time_buff;
                    break;
                }

                case MYSQL_TYPE_NEWDATE: {
                    uint32_t tmp= 0;
                    BytesHelper::readFixUint24(is, tmp);
                    current_size += 3;
                    int part;
                    char buf[11];
                    char *pos= &buf[10];  // start from '\0' to the beginning

                    /* Copied from field.cc */
                    *pos--=0;                    // End NULL
                    part=(int) (tmp & 31);
                    *pos--= (char) ('0'+part%10);
                    *pos--= (char) ('0'+part/10);
                    *pos--= ':';
                    part=(int) (tmp >> 5 & 15);
                    *pos--= (char) ('0'+part%10);
                    *pos--= (char) ('0'+part/10);
                    *pos--= ':';
                    part=(int) (tmp >> 9);
                    *pos--= (char) ('0'+part%10); part/=10;
                    *pos--= (char) ('0'+part%10); part/=10;
                    *pos--= (char) ('0'+part%10); part/=10;
                    *pos=   (char) ('0'+part);
                    value = buf;
                    break;
                }

                case MYSQL_TYPE_YEAR: {
                    uint8_t tmp= 0;
                    BytesHelper::readFixUint8(is, tmp);
                    current_size += 1;
                    char time_buff[5];
                    time_buff[5] = '\0';
                    sprintf(time_buff, "%04d", (uint32_t)tmp + 1900);
                    value = time_buff;
                    break;
                }

                case MYSQL_TYPE_ENUM: {
                    uint16_t i16 = 0;

                    switch (meta &0xFF) {
                        case 1: {
                            uint8_t tmp = 0;
                            BytesHelper::readFixUint8(is, tmp);
                            i16 = tmp;
                            current_size += 1;
                            break;
                        }

                        case 2: {
                            uint16_t tmp = 0;
                            BytesHelper::readFixUint16(is, tmp);
                            i16 = tmp;
                            current_size += 2;
                            break;
                        }
                        default:
                            break;
                    }     

                    break;
                }

                case MYSQL_TYPE_SET: {
                    uint32_t size = meta & 0xFF;
                    boost::dynamic_bitset<> my_bitset;
                    BytesHelper::readBitmap(is, my_bitset, size);
                    current_size += size;
                    boost::to_string(my_bitset, value);
                    break;
                }

                case MYSQL_TYPE_GEOMETRY:
                case MYSQL_TYPE_BLOB: {
                    uint32_t length = 0;
                    switch (meta) {
                        case 1: {
                            uint8_t tmp1 = 0;
                            BytesHelper::readFixUint8(is, tmp1);
                            length = tmp1;
                            current_size += 1;
                            break;
                        }
                        case 2: {
                            uint16_t tmp2 = 0;
                            BytesHelper::readFixUint16(is, tmp2);
                            length = tmp2;
                            current_size += 2;
                            break;
                        }
                        case 3: {
                            uint32_t tmp3 = 0;
                            BytesHelper::readFixUint24(is, tmp3);
                            length = tmp3;
                            current_size += 3;
                            break;
                        }
                        case 4: {
                            uint32_t tmp4 = 0;
                            BytesHelper::readFixUint32(is, tmp4);
                            length = tmp4;
                            current_size += 4;
                            break;
                        }
                        default:
                                break;
                    }

                    BytesHelper::readFixString(is, value, length);
                    current_size += length;
                    break;
                }

                case MYSQL_TYPE_VARCHAR:
                case MYSQL_TYPE_VAR_STRING:
                {
                    uint32_t size = meta;
                    if (size < 256) {
                        uint8_t tmp1 = 0;
                        BytesHelper::readFixUint8(is, tmp1);
                        size = tmp1;
                        current_size += 1;
                    } else {
                        uint16_t tmp2 = 0;
                        BytesHelper::readFixUint16(is, tmp2);
                        size = tmp2;
                        current_size += 2;
                    }
                    BytesHelper::readFixString(is, value, size);
                    current_size += size;
                    break;
                }

                case MYSQL_TYPE_STRING: {
                    uint32_t size = field_len;
                    if (size < 256) {
                        uint8_t tmp1 = 0;
                        BytesHelper::readFixUint8(is, tmp1);
                        current_size += 1;
                        size = tmp1;
                    } else {
                        uint16_t tmp2 = 0;
                        BytesHelper::readFixUint16(is, tmp2);
                        current_size += 2;
                        size = tmp2;
                    }
                    BytesHelper::readFixString(is, value, size);
                    current_size += size;
                    break;
                }
                default: {
                    return;
                }
            }


        }
        if (is_new) { 
            field->set_new_value(value);
        } else {
            field->set_old_value(value);
        }
        ++null_index;
    }

}
void RowsEvent::printPacket() {
    std::cout << "--------rows event packet begin---------" << std::endl;
    std::cout << "--------rows map event packet end---------" << std::endl;
}
}

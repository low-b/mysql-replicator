#include "format_description_event.h"
#include "bytes_helper.h"
#include "mysql_replicator_com.h"

namespace mysql_replicator {
FormatDescriptionEvent::FormatDescriptionEvent(uint8_t binlog_version,
    const std::string& mysql_server_version) :
    binlog_version_(binlog_version),
    mysql_server_version_(mysql_server_version),
    LogEvent(std::shared_ptr<BinlogEventHeader>()) {

    event_header_len_ = LOG_EVENT_HEADER_LEN;
    event_type_num_ = log_event_type_t::MY_END_EVENT - 1;

    switch (binlog_version_) {
        case 4: {
            static uint8_t server_event_header_length[]=
            {
                0,
                START_V3_HEADER_LEN,
                QUERY_HEADER_LEN,
                STOP_HEADER_LEN,
                ROTATE_HEADER_LEN,
                INTVAR_HEADER_LEN,
                LOAD_HEADER_LEN,

                0,
                CREATE_FILE_HEADER_LEN,
                APPEND_BLOCK_HEADER_LEN,
                EXEC_LOAD_HEADER_LEN,
                DELETE_FILE_HEADER_LEN,
                NEW_LOAD_HEADER_LEN,
                RAND_HEADER_LEN,
                USER_VAR_HEADER_LEN,
                FORMAT_DESCRIPTION_HEADER_LEN,
                XID_HEADER_LEN,
                BEGIN_LOAD_QUERY_HEADER_LEN,
                EXECUTE_LOAD_QUERY_HEADER_LEN,
                TABLE_MAP_HEADER_LEN,
                0,                                       /* PRE_GA_WRITE_ROWS_EVENT */
                0,                                       /* PRE_GA_UPDATE_ROWS_EVENT*/
                0,                                       /* PRE_GA_DELETE_ROWS_EVENT*/
                ROWS_HEADER_LEN_V1,                      /* WRITE_ROWS_EVENT_V1*/
                ROWS_HEADER_LEN_V1,                      /* UPDATE_ROWS_EVENT_V1*/
                ROWS_HEADER_LEN_V1,                      /* DELETE_ROWS_EVENT_V1*/
                INCIDENT_HEADER_LEN,
                0,                                       /* HEARTBEAT_LOG_EVENT*/
                IGNORABLE_HEADER_LEN,
                IGNORABLE_HEADER_LEN,
                ROWS_HEADER_LEN_V2,
                ROWS_HEADER_LEN_V2,
                ROWS_HEADER_LEN_V2,
                42,         /*GTID_EVENT*/
                42,         /*ANONYMOUS_GTID_EVENT*/
                IGNORABLE_HEADER_LEN,
                TRANSACTION_CONTEXT_HEADER_LEN,
                VIEW_CHANGE_HEADER_LEN,
                XA_PREPARE_HEADER_LEN
            };
            post_header_lens_.resize(event_type_num_);
            post_header_lens_.insert(post_header_lens_.begin(), server_event_header_length,
                    server_event_header_length + event_type_num_);
        }
        case 1: /* 3.23 */
        case 3: /* 4.0.x x >= 2 */ {
            event_header_len_ = binlog_version_ == 1 ? OLD_HEADER_LEN :
                LOG_EVENT_MINIMAL_HEADER_LEN;
            event_type_num_ = log_event_type_t::FORMAT_DESCRIPTION_EVENT - 1;
            static uint8_t server_event_header_length_ver_1_3[]=
            {
                0,
                START_V3_HEADER_LEN,
                QUERY_HEADER_MINIMAL_LEN,
                STOP_HEADER_LEN,
                uint8_t(binlog_version_ == 1 ? 0 : ROTATE_HEADER_LEN),
                INTVAR_HEADER_LEN,
                LOAD_HEADER_LEN,
                0,
                CREATE_FILE_HEADER_LEN,
                APPEND_BLOCK_HEADER_LEN,
                EXEC_LOAD_HEADER_LEN,
                DELETE_FILE_HEADER_LEN,
                NEW_LOAD_HEADER_LEN,
                RAND_HEADER_LEN,
                USER_VAR_HEADER_LEN
            };
            post_header_lens_.resize(event_type_num_);
            post_header_lens_.insert(post_header_lens_.begin(),
                    server_event_header_length_ver_1_3,
                    server_event_header_length_ver_1_3 +
                    event_type_num_);

            break;
        }
        default:
        break;
    }

}

void FormatDescriptionEvent::fromStream(std::istream &is) {
    BytesHelper::readFixUint16(is, binlog_version_);
    BytesHelper::readFixString(is, mysql_server_version_, 50);
    BytesHelper::readFixUint32(is, create_timestamp_);
    BytesHelper::readFixUint8(is, event_header_len_);
    std::string event_type_header_lens_string;
    BytesHelper::readEofString(is, event_type_header_lens_string);
    for (size_t i = 0; i < event_type_header_lens_string.length(); ++i) {
        post_header_lens_.push_back(event_type_header_lens_string[i]);
    }
}

void FormatDescriptionEvent::printPacket() {
    std::cout << "--------format_description event packet begin---------" << std::endl;
    std::cout << "binlog_version_:" << binlog_version_ << std::endl;
    std::cout << "mysql_server_version_:" << mysql_server_version_ << std::endl;
    std::cout << "create_timestamp_:" << create_timestamp_ << std::endl;
    std::cout << "event_header_len_:" << (uint32_t)event_header_len_ << std::endl;
    for (size_t i = 0; i < post_header_lens_.size(); ++i) {
        std::cout << "post_header_lens_[" << i << "]:" << (uint32_t)post_header_lens_[i] << std::endl;
    }
    std::cout << "--------format_description event packet end---------" << std::endl;
}
}

#ifndef  _MYSQL_REPLICATOR_COM_H_
#define  _MYSQL_REPLICATOR_COM_H_

namespace mysql_replicator {

enum enum_server_command {
  COM_SLEEP, COM_QUIT, COM_INIT_DB, COM_QUERY, COM_FIELD_LIST,
  COM_CREATE_DB, COM_DROP_DB, COM_REFRESH, COM_SHUTDOWN, COM_STATISTICS,
  COM_PROCESS_INFO, COM_CONNECT, COM_PROCESS_KILL, COM_DEBUG, COM_PING,
  COM_TIME, COM_DELAYED_INSERT, COM_CHANGE_USER, COM_BINLOG_DUMP,
  COM_TABLE_DUMP, COM_CONNECT_OUT, COM_REGISTER_SLAVE,
  COM_STMT_PREPARE, COM_STMT_EXECUTE, COM_STMT_SEND_LONG_DATA, COM_STMT_CLOSE,
  COM_STMT_RESET, COM_SET_OPTION, COM_STMT_FETCH, COM_DAEMON, COM_BINLOG_DUMP_GTID,
  COM_RESET_CONNECTION,
  /* don't forget to update const char *command_name[] in sql_parse.cc */

  /* Must be last */
  COM_END
};

namespace log_event_type_t {
    const static uint8_t UNKNOWN_EVENT = 0;
    const static uint8_t START_EVENT_V3 = 1;
    const static uint8_t QUERY_EVENT = 2;
    const static uint8_t STOP_EVENT = 3;
    const static uint8_t ROTATE_EVENT = 4;
    const static uint8_t INTVAR_EVENT = 5;
    const static uint8_t LOAD_EVENT = 6;
    const static uint8_t SLAVE_EVENT = 7;
    const static uint8_t CREATE_FILE_EVENT = 8;
    const static uint8_t APPEND_BLOCK_EVENT = 9;
    const static uint8_t EXEC_LOAD_EVENT = 10;
    const static uint8_t DELETE_FILE_EVENT = 11;
    const static uint8_t NEW_LOAD_EVENT = 12;
    const static uint8_t RAND_EVENT = 13;
    const static uint8_t USER_VAR_EVENT = 14;
    const static uint8_t FORMAT_DESCRIPTION_EVENT = 15;
    const static uint8_t XID_EVENT = 16;
    const static uint8_t BEGIN_LOAD_QUERY_EVENT = 17;
    const static uint8_t EXECUTE_LOAD_QUERY_EVENT = 18;
    const static uint8_t TABLE_MAP_EVENT = 19;
    const static uint8_t PRE_GA_WRITE_ROWS_EVENT = 20;
    const static uint8_t PRE_GA_UPDATE_ROWS_EVENT = 21;
    const static uint8_t PRE_GA_DELETE_ROWS_EVENT = 22;
    const static uint8_t WRITE_ROWS_EVENT = 23;
    const static uint8_t UPDATE_ROWS_EVENT = 24;
    const static uint8_t DELETE_ROWS_EVENT = 25;
    const static uint8_t INCIDENT_EVENT = 26;
    const static uint8_t HEARTBEAT_LOG_EVENT=27;
    const static uint8_t IGNORABLE_EVENT=28;
    const static uint8_t ROWS_QUERY_EVENT=29;
    const static uint8_t WRITE_ROWS_EVENTv2=30;
    const static uint8_t UPDATE_ROWS_EVENTv2=31;
    const static uint8_t DELETE_ROWS_EVENTv2=32;
    const static uint8_t GTID_EVENT=33;
    const static uint8_t ANONYMOUS_GTID_EVENT=34;
    const static uint8_t PREVIOUS_GTIDS_EVENT=35;
    const static uint8_t MY_END_EVENT=36;
};

const static uint8_t LOG_EVENT_HEADER_LEN = 19;
const static uint8_t OLD_HEADER_LEN = 13;
const static uint8_t LOG_EVENT_MINIMAL_HEADER_LEN = 19;
const static uint8_t ST_SERVER_VER_LEN = 50;
const static uint8_t LOG_EVENT_TYPES = log_event_type_t::MY_END_EVENT - 1;

enum enum_post_header_length {
    QUERY_HEADER_MINIMAL_LEN= (4 + 4 + 1 + 2),
    QUERY_HEADER_LEN=(QUERY_HEADER_MINIMAL_LEN + 2),
    STOP_HEADER_LEN= 0,
    LOAD_HEADER_LEN= (4 + 4 + 4 + 1 +1 + 4),
    START_V3_HEADER_LEN= (2 + ST_SERVER_VER_LEN + 4),
    ROTATE_HEADER_LEN= 8,
    INTVAR_HEADER_LEN= 0,
    CREATE_FILE_HEADER_LEN= 4,
    APPEND_BLOCK_HEADER_LEN= 4,
    EXEC_LOAD_HEADER_LEN= 4,
    DELETE_FILE_HEADER_LEN= 4,
    NEW_LOAD_HEADER_LEN= LOAD_HEADER_LEN,
    RAND_HEADER_LEN= 0,
    USER_VAR_HEADER_LEN= 0,
    FORMAT_DESCRIPTION_HEADER_LEN= (START_V3_HEADER_LEN + 1 + LOG_EVENT_TYPES),
    XID_HEADER_LEN= 0,
    BEGIN_LOAD_QUERY_HEADER_LEN= APPEND_BLOCK_HEADER_LEN,
    ROWS_HEADER_LEN_V1= 8,
    TABLE_MAP_HEADER_LEN= 8,
    EXECUTE_LOAD_QUERY_EXTRA_HEADER_LEN= (4 + 4 + 4 + 1),
    EXECUTE_LOAD_QUERY_HEADER_LEN= (QUERY_HEADER_LEN +\
            EXECUTE_LOAD_QUERY_EXTRA_HEADER_LEN),
    INCIDENT_HEADER_LEN= 2,
    HEARTBEAT_HEADER_LEN= 0,
    IGNORABLE_HEADER_LEN= 0,
    ROWS_HEADER_LEN_V2= 10,
    TRANSACTION_CONTEXT_HEADER_LEN= 18,
    VIEW_CHANGE_HEADER_LEN= 52,
    XA_PREPARE_HEADER_LEN= 0
};

}
#endif //_MYSQL_REPLICATOR_COM_H_

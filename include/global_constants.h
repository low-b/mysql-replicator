#ifndef _GLOBAL_CONSTANTS_H_
#define _GLOBAL_CONSTANTS_H_

namespace mysql_replicator {
namespace global_constants {
const static std::string SCHEMA_SQL = 
    "select schema_name from information_schema.schemata \
    where schema_name != 'information_schema'";
const static std::string TABLE_SQL =
    "select table_name from information_schema.tables \
    where table_schema='%1%' and TABLE_TYPE=\'BASE TABLE\'";
const static std::string COLOMUN_SQL =
    "select column_name,is_nullable,column_key,data_type,column_type \
    from information_schema.columns where table_schema='%1%' and table_name='%2%'";
const static std::string CLOSE_CHECKSUM_SQL =
    "SET @master_binlog_checksum='NONE'";
const static std::string VERSION_SQL =
    "SELECT VERSION()";

const static size_t PACKET_HEADER_LENTH = 4;
const static size_t EVENT_HEADER_LENTH = 20;

const static size_t PACKET_MAX_LENGTH = 256L * 256L * 256L - 1;

}
}
#endif //_GLOBAL_CONSTANTS_H_

#ifndef _GLOBAL_CONSTANTS_H_
#define _GLOBAL_CONSTANTS_H_

namespace mysql_replicator {
namespace global_constants {
const std::string SCHEMA_SQL = 
    "select schema_name from information_schema.schemata \
    where schema_name != 'information_schema'";
const std::string TABLE_SQL =
    "select table_name from information_schema.tables \
    where table_schema='%1%' and TABLE_TYPE=\'BASE TABLE\'";
const std::string COLOMUN_SQL =
    "select column_name,is_nullable,column_key,data_type \
    from information_schema.columns where table_schema='%1%' and table_name='%2%'";
}
}
#endif //_GLOBAL_CONSTANTS_H_

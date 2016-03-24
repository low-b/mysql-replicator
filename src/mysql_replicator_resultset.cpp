#include "mysql_replicator_resultset.h"

namespace mysql_replicator {
std::string MySQLReplicatorResultSet::getString(uint32_t column_index){
    return "";
}
bool MySQLReplicatorResultSet::next(){
    return true;
}
bool MySQLReplicatorResultSet::getBoolean(uint32_t column_index) {
    return true;
}
}

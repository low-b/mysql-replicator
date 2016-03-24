#ifndef __MYSQL_REPLICATOR_RESULTSET__
#define __MYSQL_REPLICATOR_RESULTSET__

#include <string>

namespace mysql_replicator {
class MySQLReplicatorResultSet {
public:
    std::string getString(uint32_t column_index); 
    bool next();
    bool getBoolean(uint32_t column_index);
};
}
#endif //__MYSQL_REPLICATOR_RESULTSET__


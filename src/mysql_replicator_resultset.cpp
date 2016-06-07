#include "mysql_replicator_resultset.h"

namespace mysql_replicator {
std::string MySQLReplicatorResultset::getString(uint32_t column_index) {
    return rows_[index_]->get_data(column_index);
}
bool MySQLReplicatorResultset::next() {
    ++index_;
    return index_ < rows_.size();
}
}

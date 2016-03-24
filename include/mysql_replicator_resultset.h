#ifndef __MYSQL_REPLICATOR_RESULTSET__
#define __MYSQL_REPLICATOR_RESULTSET__

#include <string>
#include <vector>
#include <memory>
#include "column_definition_packet.h"
#include "resultset_row_packet.h"

namespace mysql_replicator {
class MySQLReplicatorResultset {
public:
    MySQLReplicatorResultset():index_(0) {}
    void addColumn(std::shared_ptr<ColumnDefinitionPacket> column) {
        columns_.push_back(column);
    }
    void addRow(std::shared_ptr<ResultsetRowPacket> row) {
        rows_.push_back(row);
    }
    std::string getString(uint32_t column_index);
    bool next();
    bool getBoolean(uint32_t column_index);
    void set_column_count(uint64_t column_count) {
        column_count_ = column_count;
    }
private:
    uint64_t column_count_;
    std::vector<std::shared_ptr<ColumnDefinitionPacket> > columns_;
    std::vector<std::shared_ptr<ResultsetRowPacket> > rows_;
    uint64_t index_;
};
}
#endif //__MYSQL_REPLICATOR_RESULTSET__


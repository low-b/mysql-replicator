#ifndef  __DB_META_H_
#define  __DB_META_H_

#include <memory>
#include "mysql_replicator_com.h"
#include "mysql_replicator_connector.h"

namespace mysql_replicator {
class ColumnMeta {
public:
    void set_column_name(const std::string column_name) {
        column_name_ = column_name;
    }
    void set_is_nullable(bool is_nullable) {
        is_nullable_ = is_nullable;
    }
    void set_is_pk(const std::string is_pk) {
        if (is_pk == "PRI") {
            is_pk_ = true;
        } else {
            is_pk_  = false;
        }
    }
    void set_data_type(const std::string data_type) {
        data_type_ = data_type;
    }
private:
    std::string column_name_;
    bool is_nullable_;
    bool is_pk_;
    std::string data_type_;
//    enum_field_types data_type_;
//    std::string charset_;
};

class TableMeta {
public:
    void build(const std::string& schema_name,
            const std::string& table_name,
            std::shared_ptr<MySQLReplicatorConnector> conn);
private:
    std::string schema_name_;
    std::string table_name_;
    std::vector<std::shared_ptr<ColumnMeta> > columns_;
//    std::string charset_;
};

class SchemaMeta {
public:
    void build(const std::string& schema_name, std::shared_ptr<MySQLReplicatorConnector> conn);
private:
    std::map<std::string, std::shared_ptr<TableMeta> > tables_;
    std::string schema_name_;
};

class DbMeta {
public:
    void build(std::shared_ptr<MySQLReplicatorConnector> conn);
private:
    std::map<std::string, std::shared_ptr<SchemaMeta> > schemas_;
//    std::map<std::string, TableMeta> tables;
};
}

#endif  //__DB_META_H_

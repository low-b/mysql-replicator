#ifndef  __DB_META_H_
#define  __DB_META_H_

#include <memory>
#include "mysql_replicator_com.h"
#include "mysql_replicator_connector.h"
#include "binlog_event.pb.h"

namespace mysql_replicator {
class ColumnMeta {
public:
    void set_column_name(const std::string column_name) {
        column_name_ = column_name;
    }
    std::string get_column_name() {
        return column_name_;
    }
    void set_is_nullable(bool is_nullable) {
        is_nullable_ = is_nullable;
    }
    void set_is_signed(bool is_signed) {
        is_signed_ = is_signed;
    }
    bool get_is_signed() {
        return is_signed_;
    }
    void set_is_pk(const std::string is_pk) {
        if (is_pk == "PRI") {
            is_pk_ = true;
        } else {
            is_pk_  = false;
        }
    }
    bool get_is_pk() {
        return is_pk_;
    }
    void set_data_type(const std::string data_type);
    MysqlType get_data_type() {
        return data_type_;
    }
private:
    std::string column_name_;
    bool is_nullable_;
    bool is_pk_;
    MysqlType data_type_;
    bool is_signed_;
//    std::string charset_;
};

class TableMeta {
public:
    void build(const std::string& schema_name,
            const std::string& table_name,
            std::shared_ptr<MySQLReplicatorConnector> conn);
    std::shared_ptr<ColumnMeta> at(size_t index) {
        return columns_[index];
    }
private:
    std::string schema_name_;
    std::string table_name_;
    std::vector<std::shared_ptr<ColumnMeta> > columns_;
//    std::string charset_;
};

class SchemaMeta {
public:
    void build(const std::string& schema_name, std::shared_ptr<MySQLReplicatorConnector> conn);
    std::shared_ptr<TableMeta> get_table(const std::string table_name) {
        return tables_[table_name];
    }
private:
    std::map<std::string, std::shared_ptr<TableMeta> > tables_;
    std::string schema_name_;
};

class DbMeta {
public:
    void build(std::shared_ptr<MySQLReplicatorConnector> conn);
    std::shared_ptr<TableMeta> get_table(const std::string schema_name,
            const std::string table_name) {
        if (schemas_.find(schema_name) == schemas_.end()) {
            return std::shared_ptr<TableMeta>();
        }
        return schemas_[schema_name]->get_table(table_name);
    }
private:
    std::map<std::string, std::shared_ptr<SchemaMeta> > schemas_;
//    std::map<std::string, TableMeta> tables;
};
}

#endif  //__DB_META_H_

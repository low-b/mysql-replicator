#include "db_meta.h"
#include <boost/format.hpp>
#include "mysql_replicator_statement.h"
#include "mysql_replicator_resultset.h"
#include "global_constants.h"

using std::string;
using std::shared_ptr;
namespace mysql_replicator {

/*
void ColumnMeta::set_data_type(const std::string data_type) {
//    if (date_type == "decimal") {
//        data_type_ = MYSQL_TYPE_DECIMAL;
    if (data_type == "tinyint") {
        data_type_ = MYSQL_TYPE_TINY;
    } else if (data_type == "smallint") {
        data_type_ = MYSQL_TYPE_SHORT;
    } else if (data_type == "int") {
        data_type_ = MYSQL_TYPE_LONG;
    } else if (data_type == "float") {
        data_type_ = MYSQL_TYPE_FLOAT;
    } else if (data_type == "double") {
        data_type_ = MYSQL_TYPE_DOUBLE;
//    } else if (data_type == "") {
//        data_type_ = MYSQL_TYPE_NULL;
    } else if (data_type == "timestamp") {
        data_type_ = MYSQL_TYPE_TIMESTAMP;
    } else if (data_type == "bigint") {
        data_type_ = MYSQL_TYPE_LONGLONG;
    } else if (data_type == "mediumint") {
        data_type_ = MYSQL_TYPE_INT24;
    } else if (data_type == "date") {
        data_type_ = MYSQL_TYPE_DATE;
    } else if (data_type == "time") {
        data_type_ = MYSQL_TYPE_TIME;
    } else if (data_type == "datetime") {
        data_type_ = MYSQL_TYPE_DATETIME;
    } else if (data_type == "year") {
        data_type_ = MYSQL_TYPE_YEAR;
//    } else if (data_type == "") {
//        data_type_ = MYSQL_TYPE_NEWDATE;
    } else if (data_type == "varchar") {
        data_type_ = MYSQL_TYPE_VARCHAR;
    } else if (data_type == "bit") {
        data_type_ = MYSQL_TYPE_BIT;
    } else if (data_type == "decimal") {
        data_type_ = MYSQL_TYPE_NEWDECIMAL;
    } else if (data_type == "enum") {
        data_type_ = MYSQL_TYPE_ENUM;
    } else if (data_type == "set") {
        data_type_ = MYSQL_TYPE_SET;
    } else if (data_type == "tinyblob") {
        data_type_ = MYSQL_TYPE_TINY;
    } else if (data_type == "") {
        data_type_ = MYSQL_TYPE_TINY;
    } else if (data_type == "") {
        data_type_ = MYSQL_TYPE_TINY;
    } else if (data_type == "") {
        data_type_ = MYSQL_TYPE_TINY;
    data_type_ = data_type;
}
*/

void TableMeta::build(const std::string& schema_name,
        const std::string& table_name,
        std::shared_ptr<MySQLReplicatorConnector> conn) {
    string sql = boost::str(
            boost::format(global_constants::COLOMUN_SQL) % schema_name % table_name);
    shared_ptr<MySQLReplicatorStatement> state(conn->createStatement());
    shared_ptr<MySQLReplicatorResultset> result(state->executeQuery(sql));
    while (result->next()) {
        string column_name = result->getString(1);
        shared_ptr<ColumnMeta> c(new ColumnMeta);
        c->set_column_name(column_name);
        c->set_is_nullable(result->getBoolean(2));
        c->set_is_pk(result->getString(3));
        c->set_data_type(result->getString(4));
        columns_.push_back(c);
    }
}
void SchemaMeta::build(const std::string& schema_name,
        std::shared_ptr<MySQLReplicatorConnector> conn) {
    shared_ptr<MySQLReplicatorStatement> state(conn->createStatement());
    string sql = boost::str(
            boost::format(global_constants::TABLE_SQL) % schema_name);
    shared_ptr<MySQLReplicatorResultset> result(state->executeQuery(sql));
    while (result->next()) {
        string table_name = result->getString(1);
        shared_ptr<TableMeta> t(new TableMeta);
        t->build(schema_name, table_name, conn);
        tables_.insert(std::make_pair(table_name, t));
    }
}
void DbMeta::build(std::shared_ptr<MySQLReplicatorConnector> conn) {
    shared_ptr<MySQLReplicatorStatement> state(conn->createStatement());
    shared_ptr<MySQLReplicatorResultset> result(state->executeQuery(global_constants::SCHEMA_SQL));
    while (result->next()) {
        string schema_name = result->getString(1);
        shared_ptr<SchemaMeta> s(new SchemaMeta);
        s->build(schema_name, conn);
        schemas_.insert(std::make_pair(schema_name, s));
    }
}
}

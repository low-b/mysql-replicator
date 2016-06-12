#include "network_driver.h"
#include "mysql_replicator_statement.h"
#include "mysql_replicator_resultset.h"
#include "global_constants.h"
#include "format_description_event.h"
#include "binlog_event_factory.h"
#include "binlog_dump_exception.h"
#include "binlog_event.pb.h"
#include "mysql_replicator_com.h"
#include "rows_event.h"
#include "rotate_event.h"
using std::shared_ptr;
using std::string;

#include <iostream>
using namespace std;

namespace mysql_replicator {
void NetworkDriver::init() {
    conn_ = std::make_shared<MySQLReplicatorConnector>(
            host_, port_, username_, password_);
    conn_->connect();

    db_meta_ = shared_ptr<DbMeta>(new DbMeta);
    db_meta_->build(conn_);

    event_factory_ = std::make_shared<BinlogEventFactory>();
    event_factory_->set_db_meta(db_meta_);

    shared_ptr<MySQLReplicatorStatement> state(conn_->createStatement());
    state->executeQuery(global_constants::CLOSE_CHECKSUM_SQL);

    shared_ptr<MySQLReplicatorResultset> result(state->executeQuery(global_constants::VERSION_SQL));
    string version;
    if (result->next()) {
        version = result->getString(1);
    }

    switch (version[0]) {
        case '3':
            event_factory_->set_format_description_event(std::make_shared<FormatDescriptionEvent>(1, version));
            break;
        case '4':
        case '5':
            event_factory_->set_format_description_event(std::make_shared<FormatDescriptionEvent>(3, version));
            break;
        default:
            throw BinlogDumpException("unsupported mysql version:[" + version + "]");
            break;
    }

    dumper_ = std::make_shared<MySQLReplicatorDumper>(conn_->get_socket(), event_factory_);
}

void NetworkDriver::sendBinlogDump(std::shared_ptr<BinlogCheckPoint> binlog_pos) {
    check_point_ = binlog_pos;
    dumper_->sendBinlogDump(binlog_pos);
}

void NetworkDriver::takeBinlogEvent(std::vector<std::shared_ptr<BinlogEvent> >& event_vec) {
    shared_ptr<LogEvent> event = dumper_->takeBinlogEvent();
    shared_ptr<BinlogEventHeader> header = event->get_header();
    check_point_->process(event);
    switch (header->event_type) {
        case log_event_type_t::WRITE_ROWS_EVENT:
        case log_event_type_t::UPDATE_ROWS_EVENT:
        case log_event_type_t::DELETE_ROWS_EVENT:
        case log_event_type_t::WRITE_ROWS_EVENTv2:
        case log_event_type_t::UPDATE_ROWS_EVENTv2:
        case log_event_type_t::DELETE_ROWS_EVENTv2: {
            shared_ptr<RowsEvent> rows_event = std::dynamic_pointer_cast<RowsEvent>(event);
            rows_event->buildEventProto(event_vec);
            for (auto &event : event_vec) {
                Status *status = event->mutable_status();
                status->set_log_file(check_point_->get_binlog_filename());
                status->set_offset(header->log_pos);
                status->set_timestamp(header->timestamp);
                status->set_host(host_);
                status->set_port(port_);
                status->set_progress(check_point_->get_progress());
            }
            return;
        }
    }
}

}

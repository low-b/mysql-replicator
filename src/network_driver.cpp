#include "network_driver.h"
#include "mysql_replicator_statement.h"
#include "mysql_replicator_resultset.h"
#include "global_constants.h"
#include "format_description_event.h"
#include "binlog_event_factory.h"
#include "binlog_dump_exception.h"
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
    dumper_->sendBinlogDump(binlog_pos);

}
std::shared_ptr<BinlogEvent> NetworkDriver::takeBinlogEvent() {
    std::shared_ptr<LogEvent> event;
    do {
            event = dumper_->takeBinlogEvent();
    } while (1);
    //while (1) {
    //    std::shared_ptr<EventHeader> header = get_header();
    //    event = parse_event(is, header);
    //    for (size_t i = 0; i < handlers_.size(); ++i) {
    //        if (!event) {
    //            break;
    //        }
    //        handlers_[i].handle(event);
    //    }
    //    if (event) {
    //        break;
    //    }
    //}
    return std::shared_ptr<BinlogEvent>();
}

}

#include "network_driver.h"
#include "mysql_replicator_statement.h"
#include "mysql_replicator_resultset.h"
using std::shared_ptr;
namespace mysql_replicator {
void NetworkDriver::init() {
    conn_ = std::make_shared<MySQLReplicatorConnector>(
            host_, port_, username_, password_);
    conn_->connect();
    db_meta_ = shared_ptr<DbMeta>(new DbMeta);
    db_meta_->build(conn_);
    dumper_ = std::make_shared<MySQLReplicatorDumper>(conn_->get_socket());
}

void NetworkDriver::sendBinlogDump(std::shared_ptr<BinlogCheckPoint> binlog_pos) {
    dumper_->sendBinlogDump(binlog_pos);

}
std::shared_ptr<binlog_listener::BinlogEvent> NetworkDriver::takeBinlogEvent() {
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
    return std::shared_ptr<binlog_listener::BinlogEvent>();
}

}

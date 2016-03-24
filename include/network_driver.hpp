using std::shared_ptr;
#include "mysql_replicator_statement.h"
#include "mysql_replicator_resultset.h"
namespace mysql_replicator {
template<typename BinlogStartPoint, typename BinlogParser>
void
NetworkDriver<BinlogStartPoint, BinlogParser>::
init() {
    conn_->connect();
    printf("aaaaa\n");
    shared_ptr<MySQLReplicatorStatement> state(conn_->createStatement());
    printf("bbbbb\n");
    shared_ptr<MySQLReplicatorResultSet> result(state->executeQuery("set names gb"));
    printf("ccccc\n");
    //db_meta_ = shared_ptr<DbMeta>(new DbMeta);
    //db_meta_->build(conn_);
    //start_point_.send_binlog_dump();

}

template<typename BinlogStartPoint, typename BinlogParser>
std::shared_ptr<BinlogEvent>
NetworkDriver<BinlogStartPoint, BinlogParser>::
takeBinlogEvent() {
    std::shared_ptr<BinlogEvent> event;
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
    return event;
}

}

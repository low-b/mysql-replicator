#ifndef  __MYSQL_REPLICATOR_DUMPER_H__
#define  __MYSQL_REPLICATOR_DUMPER_H__

#include <string>
#include <unordered_set>
#include <boost/asio.hpp>
#include "binlog_check_point.h"
#include "log_event.h"
#include "mysql_replicator_com.h"
#include "db_meta.h"
#include "binlog_event_factory.h"

namespace mysql_replicator {
class MySQLReplicatorDumper {
public:
    MySQLReplicatorDumper(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
            std::shared_ptr<BinlogEventFactory> event_factory)
        : socket_(socket), event_factory_(event_factory) { }
    void sendBinlogDump(std::shared_ptr<BinlogCheckPoint> binlog_pos);
    std::shared_ptr<LogEvent> takeBinlogEvent();
private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<BinlogEventFactory> event_factory_;
};
}

#endif  //__MYSQL_REPLICATOR_DUMPER_H__

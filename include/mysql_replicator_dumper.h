#ifndef  __MYSQL_REPLICATOR_DUMPER_H__
#define  __MYSQL_REPLICATOR_DUMPER_H__

#include <string>
#include <unordered_set>
#include <boost/asio.hpp>
#include "binlog_check_point.h"
#include "log_event.h"
#include "mysql_replicator_com.h"

namespace mysql_replicator {
class MySQLReplicatorDumper {
public:
    MySQLReplicatorDumper(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
        : socket_(socket) { }
    void sendBinlogDump(std::shared_ptr<BinlogCheckPoint> binlog_pos);
    std::shared_ptr<LogEvent> takeBinlogEvent();
private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    //std::unordered_set<uint8_t> not_handle_set_;
};
}

#endif  //__MYSQL_REPLICATOR_DUMPER_H__

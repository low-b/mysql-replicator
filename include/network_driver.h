#ifndef  __NETWORK_DRIVER_H_
#define  __NETWORK_DRIVER_H_

#include <string>
#include <map>
#include <vector>
#include "binlog_event.pb.h"
#include "mysql_replicator_connector.h"
#include "mysql_replicator_dumper.h"
#include "db_meta.h"
#include "binlog_check_point.h"

namespace mysql_replicator {

class NetworkDriver {
public: 
    NetworkDriver(const std::string& host, uint16_t port,
            const std::string& username,
            const std::string& password):
        host_(host), port_(port), username_(username),
        password_(password) {
        }
    void init();
    void sendBinlogDump(std::shared_ptr<BinlogCheckPoint> binlog_pos);
    std::shared_ptr<binlog_listener::BinlogEvent> takeBinlogEvent();
private:
    std::shared_ptr<DbMeta> db_meta_;
    std::shared_ptr<MySQLReplicatorConnector> conn_;
    std::shared_ptr<MySQLReplicatorDumper> dumper_;
    std::string host_;
    uint16_t port_;
    std::string username_;
    std::string password_;

};
}

#endif  //__NETWORK_DRIVER_H_
